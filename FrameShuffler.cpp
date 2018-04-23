// FrameShuffler software implemented by Miroslav Goncharenko, miroslav.goncharenko@intel.com

#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <string.h>
#include <random>
#include <numeric>

using namespace std;

inline int RoundUpModulo64(int val) { return ((val + 63) >> 6) << 6; }

class InputParser
{
public:
	InputParser(int argc, char **argv)
	{
		m_tokens.reserve(argc - 1);

		for (int i = 1; i < argc; ++i)
			m_tokens.emplace_back(argv[i]);
	}


	const string& getCmdOption(const string &option) const
	{
		auto it = find(m_tokens.begin(), m_tokens.end(), option);
		if (it != m_tokens.end() && ++it != m_tokens.end())
		{
			return *it;
		}
		static const string empty_string("");
		return empty_string;
	}

	bool cmdOptionExists(const string &option) const
	{
		return find(m_tokens.begin(), m_tokens.end(), option)
			!= this->m_tokens.end();
	}

private:
	vector <string> m_tokens;
};

void ProcessStream(const string & input_filename, const string & output_filename, int frame_width, int frame_height, int seed);

int main(int argc, char * argv[])
{
	try
	{
		InputParser input(argc, argv);

		if (input.cmdOptionExists("-?"))
		{
			cout << "This program generates YUV file with random shuffled 64x64 blocks within each frame (except first one).\n"
				<< "Available options:\n"
				<< "    -?              - prints this message;\n"
				<< "    -i    <path>    - input YUV file name;\n"
				<< "    -o    <path>    - output YUV file name;\n"
				<< "    -w    <width>   - width of frame;\n"
				<< "    -h    <height>  - height of frame;\n"
				<< "    -seed <seed>    - set seed;" << endl;

			return 0;
		}

		string input_filename = input.getCmdOption("-i");
		if (input_filename.empty())
		{
			throw string("No input file name provided");
		}

		string output_filename = input.getCmdOption("-o");
		if (output_filename.empty())
		{
			throw string("No output file name provided");
		}

		string tmp = input.getCmdOption("-w");
		if (tmp.empty())
		{
			throw string("No width of YUV frame parameter provided");
		}
		int frame_width = stoi(tmp);

		tmp = input.getCmdOption("-h");
		if (tmp.empty())
		{
			throw string("No height of YUV frame parameter provided");
		}
		int frame_height = stoi(tmp);

		int seed = 0;
		tmp = input.getCmdOption("-seed");
		if (!tmp.empty())
		{
			seed = stoi(tmp);
		}

		ProcessStream(input_filename, output_filename, frame_width, frame_height, seed);
	}
	catch (const string& str)
	{
		cout << str << endl;
	}
	catch (...)
	{
		cout << "Some unexpected exception thrown" << endl;
	}

	return 0;
}

class YUVframe
{
public:
	YUVframe(int w = 0, int h = 0)
		: crop_w_Y(w)
		, crop_h_Y(h)
		, width_Y(RoundUpModulo64(crop_w_Y))
		, height_Y(RoundUpModulo64(crop_h_Y))
		, crop_w_UV((crop_w_Y + 1) >> 1)
		, crop_h_UV((crop_h_Y + 1) >> 1)
		, width_UV((width_Y + 1) >> 1)
		, height_UV((height_Y + 1) >> 1)
		, pitch_Y(width_Y)
		, pitch_UV(width_UV)
		, pixel_data(width_Y*height_Y + (width_UV * height_UV) * 2, 0)
	{
		fill(pixel_data.data() + width_Y*height_Y, pixel_data.data() + width_Y*height_Y + (width_UV * height_UV) * 2, 128);
	}

	void ReadFrame(ifstream & input)
	{
		char *pdata = pixel_data.data();
		for (int i = 0; i < crop_h_Y; ++i)
		{
			input.read(pdata, crop_w_Y);
			pdata += pitch_Y;
		}

		pdata = pixel_data.data() + width_Y*height_Y;
		for (int i = 0; i < crop_h_UV; ++i)
		{
			input.read(pdata, crop_w_UV);
			pdata += pitch_UV;
		}

		pdata = pixel_data.data() + width_Y*height_Y + width_UV*height_UV;
		for (int i = 0; i < crop_h_UV; ++i)
		{
			input.read(pdata, crop_w_UV);
			pdata += pitch_UV;
		}
	}

	void WriteFrame(ofstream & output)
	{
		output.write(pixel_data.data(), pixel_data.size());
		output.flush();
	}

	int crop_w_Y = -1;
	int crop_h_Y = -1;

	int width_Y = -1;
	int height_Y = -1;

	int crop_w_UV = -1;
	int crop_h_UV = -1;

	int width_UV = -1;
	int height_UV = -1;

	int pitch_Y = -1;
	int pitch_UV = -1;

	vector<char> pixel_data;
};

void Copy64Block(YUVframe & input_frame, unsigned int input_idx, YUVframe & output_frame, unsigned int output_idx)
{
	// Copy luma 64x64 block
	unsigned int block_size = 64;

	unsigned int input_lines_to_skip   = (input_idx*block_size) / input_frame.width_Y;
	unsigned int output_lines_to_skip  = (output_idx*block_size) / output_frame.width_Y;

	unsigned int input_offset_in_line  = (input_idx*block_size) % input_frame.width_Y;
	unsigned int output_offset_in_line = (output_idx*block_size) % output_frame.width_Y;

	unsigned int input_pixels_to_skip  = input_lines_to_skip*input_frame.pitch_Y*block_size;
	unsigned int output_pixels_to_skip = output_lines_to_skip*output_frame.pitch_Y*block_size;

	unsigned int input_start_pos  = input_pixels_to_skip + input_offset_in_line;
	unsigned int output_start_pos = output_pixels_to_skip + output_offset_in_line;

	for (unsigned int i = 0; i < block_size; ++i)
	{
		memcpy(output_frame.pixel_data.data() + output_start_pos, input_frame.pixel_data.data() + input_start_pos, block_size);
		input_start_pos  += input_frame.pitch_Y;
		output_start_pos += output_frame.pitch_Y;
	}

	// Copy two chroma 32x32 blocks
	block_size >>= 1;

	input_pixels_to_skip  >>= 2;
	output_pixels_to_skip >>= 2;

	input_offset_in_line  >>= 1;
	output_offset_in_line >>= 1;

	// Cb
	unsigned int input_pitch_in_chroma  = input_pixels_to_skip + input_offset_in_line;
	unsigned int output_pitch_in_chroma = output_pixels_to_skip + output_offset_in_line;

    input_start_pos  = input_frame.width_Y * input_frame.height_Y + input_pitch_in_chroma;
	output_start_pos = output_frame.width_Y * output_frame.height_Y + output_pitch_in_chroma;

	for (unsigned int i = 0; i < block_size; ++i)
	{
		memcpy(output_frame.pixel_data.data() + output_start_pos, input_frame.pixel_data.data() + input_start_pos, block_size);
		input_start_pos  += input_frame.pitch_UV;
		output_start_pos += output_frame.pitch_UV;
	}

	// Cr
	input_start_pos  = input_frame.width_Y * input_frame.height_Y + input_frame.width_UV * input_frame.height_UV + input_pitch_in_chroma;
	output_start_pos = output_frame.width_Y * output_frame.height_Y + output_frame.width_UV * output_frame.height_UV + output_pitch_in_chroma;

	for (unsigned int i = 0; i < block_size; ++i)
	{
		memcpy(output_frame.pixel_data.data() + output_start_pos, input_frame.pixel_data.data() + input_start_pos, block_size);
		input_start_pos  += input_frame.pitch_UV;
		output_start_pos += output_frame.pitch_UV;
	}
}

void output_shuffle(vector<unsigned int> & permutation)
{
	for (auto p : permutation)
	{
		cout << p << " ";
	}
	cout << endl;
}

void ProcessStream(const string & input_filename, const string & output_filename, int frame_width, int frame_height, int seed)
{
	ifstream input(input_filename, ios::binary);
	if (!input.is_open())
	{
		throw string("Failed to open input file ") + input_filename;
	}

	ofstream output(output_filename, ios::binary | ios::trunc);
	if (!output.is_open())
	{
		throw string("Failed to open output file ") + output_filename;
	}

	YUVframe input_frame(frame_width, frame_height), output_frame(frame_width, frame_height);

	// Copy first frame
	input_frame.ReadFrame(input);
	input_frame.WriteFrame(output);

	// Create linear indexing for shuffling
	vector<unsigned int> idx((RoundUpModulo64(frame_width) >> 6)*(RoundUpModulo64(frame_height) >> 6));
	iota(idx.begin(), idx.end(), 0);

	// Create random device
	random_device rd;
	mt19937 g(rd());
	g.seed(seed);

	int frame_number = 0;

	// Read frame
	input_frame.ReadFrame(input);
	while (!input.eof())
	{
		// Create blocks permutation
		shuffle(idx.begin(), idx.end(), g);
		cout << "Frame " << frame_number++ << " : ";
		output_shuffle(idx);

		for (unsigned int i = 0; i < idx.size(); ++i)
		{
			Copy64Block(input_frame, i, output_frame, idx[i]);
		}

		// Write output frame
		output_frame.WriteFrame(output);

		// Read frame
		input_frame.ReadFrame(input);
	}

	input.close();
	output.close();
}
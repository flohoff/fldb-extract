#include <iostream>
#include <fstream>

#include <sstream>
#include <string.h>

#include "byteorder.h"

#include <boost/program_options.hpp>
#include <string>

namespace po = boost::program_options;

class FLDB_Header {
	struct {
		uint32_t	possible_directory_offset;
		uint32_t	unknown1[4];
		char		identifier[4];
		uint32_t	unknown2[2];
	} _header;

	public:

	bool is_valid_fldb(void) {
		return (0 == strncmp("FLDB", _header.identifier, 4));
	}

	off_t directory_offset(void) {
		return le32_to_cpu(_header.possible_directory_offset);
	}

	void from_stream(std::ifstream& f) {
		f.seekg(0);
		f.read ((char *) &_header,sizeof(_header));
	}
};

class FLDB_Directory_Entry {
	struct {
		uint32_t	offset;
		uint32_t	length;
		char		name[24];
		uint32_t	unknown2;
	} _entry;

	public:

	std::string	name;

	void from_stream(std::ifstream& f) {
		f.read((char *) &_entry, sizeof(_entry));

		name=_entry.name;
	}

	uint32_t offset(void) {
		return le32_to_cpu(_entry.offset);
	}

	uint32_t length(void) {
		return le32_to_cpu(_entry.length);
	}

	bool is_valid() {
		return offset() != 0;
	}
};

class FLDB_Directory {
	public:
	std::vector<FLDB_Directory_Entry*>	files;

	void from_stream(std::ifstream& f, FLDB_Header& header) {
		FLDB_Directory_Entry *de;

		f.seekg(header.directory_offset());
		while(42) {
			de=new(FLDB_Directory_Entry);
			de->from_stream(f);

			if (!de->is_valid()) {
				break;
			}

			std::cout << de->name << " Offset: " << de->offset() << " Length: " << de->length() << std::endl;

			files.push_back(de);
		}
	}

};

class FLDB {
	std::string filename;
	std::ifstream file;
	FLDB_Header	header;
	FLDB_Directory directory;

	public:

	FLDB(std::string filename) : filename(filename),file(filename, std::ifstream::binary) {
		header_read();
		directory_read();
	}

	void directory_read(void ) {
		directory.from_stream(file, header);
	}

	void header_read(void ) {
		header.from_stream(file);

		if (header.is_valid_fldb()) {
			std::cout << "File is an FLDB file" << std::endl;
			return;
		}

		std::cerr << "File is not an FLDB file" << std::endl;
		exit(0);
	}

	void extract_files(std::string outputdir ) {
		for(auto de : directory.files) {
			std::string filename = outputdir;
			filename.append("/");
			filename.append(de->name);

			file.seekg(de->offset());

			std::ofstream of(filename, std::fstream::trunc|std::fstream::binary);

			char	buffer[1024];
			int	len=de->length();

			while(len > 0) {
				int todo=(len > 1024) ? 1024 : len;
				file.read(buffer, todo);
				of.write(buffer, todo);
				len-=todo;
			}
		}
	}
};

int main(int argc, char **argv) {
	bool t_extract=false;

	po::options_description         desc("Allowed options");
	desc.add_options()
		("help,h", "produce help message")
		("infile,i", po::value<std::string>()->required(), "Input file")
		("outdir,o", po::value<std::string>()->required(), "Output directory")
		("extract,x", po::bool_switch(&t_extract), "Do not use boundary caching")
	;

	po::variables_map vm;
	try {
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);
	} catch(const boost::program_options::error& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		std::cerr << desc << std::endl;
		exit(-1);
	}

	FLDB fldb(vm["infile"].as<std::string>());

	if (t_extract) {
		fldb.extract_files(vm["outdir"].as<std::string>());
	}
}

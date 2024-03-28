#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <variant>
#include <vector>
int main()
{
	std::fstream i_dat("zgradi_nacrt.txt");
	std::vector<std::string> za_ciscenje;
	std::string line;
	std::string build_mapa,ime_projekta,entry;

	i_dat>>line;
	i_dat>>line;
	i_dat>>ime_projekta;
	i_dat>>line;
	i_dat>>line;
	i_dat>>build_mapa;
	i_dat>>line;
	i_dat>>line;
	i_dat>>entry;
	std::cout<<ime_projekta<<"  "<<build_mapa<<"  "<<entry<<"\n";
	while(std::getline(i_dat, line))
	{
		if(line == "NASM_bin = {")
		{
			line.pop_back();
			line.pop_back();
			line.pop_back();
			while(true)
			{
				std::string nasm_asm, nasm_bin;
				std::getline(i_dat,nasm_asm);
	
				if(nasm_asm == "}")
					break;
				
				std::cout<<line<<": "<<nasm_asm<<"\n";
				
				nasm_bin = nasm_asm;
				nasm_bin.pop_back();
				nasm_bin.pop_back();
				nasm_bin.pop_back();
				nasm_bin += "bin";
				
				za_ciscenje.push_back(nasm_bin);

				std::string ukaz = "nasm -f bin " + nasm_asm + " -o " + nasm_bin;

				std::system(ukaz.c_str());
			}	
		}
		if(line == "C_o = {")
		{
			line.pop_back();
			line.pop_back();
			line.pop_back();
			while(true)
			{
				std::string c_c,c_o;
				std::getline(i_dat,c_c);
				if(c_c == "}")
					break;
				
				std::cout<<line<<": "<<c_c<<"\n";
				
				c_o = c_c;
				c_o.pop_back();
				c_o += "o";
				
				za_ciscenje.push_back(c_o);
				
				std::string ukaz = "i686-elf-gcc -ffreestanding -c " + c_c + " -o " + c_o;

				std::system(ukaz.c_str());
			}
		}
		if(line == "NASM_o = {")
		{
			line.pop_back();
			line.pop_back();
			line.pop_back();
			while(true)
			{
				std::string nasm_asm, nasm_o;
				std::getline(i_dat,nasm_asm);
				if(nasm_asm == "}")
					break;
				std::cout<<line<<": "<<nasm_asm<<"\n";
				nasm_o = nasm_asm;
				nasm_o.pop_back();
				nasm_o.pop_back();
				nasm_o.pop_back();
				nasm_o += "o";

				za_ciscenje.push_back(nasm_o);
				std::string ukaz = "nasm -f elf " + nasm_asm + " -o " + nasm_o;
				std::system(ukaz.c_str());
			}
	}
	if(line == "LINK_bin = {")
	{
		line.pop_back();
		line.pop_back();
		line.pop_back();
		std::vector<std::string> link_seznam;
		while(true)
		{
			std::string link;
			std::getline(i_dat,link);
			if(link == "}")
				break;
			if(link.back() == 'm')
			{
				link.pop_back();
				link.pop_back();
				link.pop_back();
			}
			else
			{
				link.pop_back();
			}
			link.push_back('o');
			link_seznam.push_back(link);
			std::cout<<line<<": "<<link<<"\n";
		}
		std::string izhod = build_mapa + "/";
		izhod+=ime_projekta;
		izhod+="_tmp.bin";
		za_ciscenje.push_back(izhod);
		std::string ukaz = "i686-elf-ld -o " + izhod + " -Ttext " + entry;
	   	for(int i = 0; i < link_seznam.size(); i++)
		{
			ukaz += " " + link_seznam[i];
		}	
		ukaz += " --oformat binary";
		std::system(ukaz.c_str());
	}
	if(line == "CAT_bin = {")
	{
		line.pop_back();
		line.pop_back();
		line.pop_back();
		std::vector <std::string> cat_seznam;
		while(true)
		{
			std::string cat;
			std::getline(i_dat,cat);
			if(cat == "}")
				break;
			else if(cat == "LINK_bin")
			{
				cat = build_mapa + "/" + ime_projekta;
			 	cat += "_tmp.bin";
			}
			else
			{
				cat.pop_back();
				cat.pop_back();
				cat.pop_back();
				cat += "bin";
			}
			cat_seznam.push_back(cat);
			std::cout<<line<<": "<<cat<<"\n";
		}
		std::string ukaz = "cat ";
		for(int i=0;i<cat_seznam.size();i++)
				ukaz += " " + cat_seznam[i];

		ukaz += " > ";
		ukaz += build_mapa +"/";
		ukaz += ime_projekta+".bin";
		std::system(ukaz.c_str());
	}

}
	for(int i=0;i<za_ciscenje.size();i++)
	{
		std::remove(za_ciscenje[i].c_str());
		std::cout<<"RM: "<< za_ciscenje[i]<<"\n";
	}

	
}

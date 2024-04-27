#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <type_traits>
#include <vector>
#include <string>

std::vector <std::string> kernel_c =
{
	"src/kernel/kernel.c",
	"src/kernel/preprost_vga_gonilnik/preprost_vga_gonilnik.c",
	"src/kernel/c_lib/string.c",
	"src/kernel/c_lib/stdio.c"
};

std::string linker_ld = "src/grub_zagon/grub_linker.ld ";
std::string grub_s = "src/grub_zagon/grub.s ";
std::string projekt = "build/gallus_os";

std::vector<std::string> odvecne_dat;

std::string cc = "i686-elf-gcc -std=gnu99 -ffreestanding -O2 -Wall -Wextra ";
std::string s = "i686-elf-as ";
std::string ld = "i686-elf-gcc -ffreestanding -O2 -nostdlib -lgcc ";

void naredi_kernel()
{
	for(int i = 0; i < kernel_c.size(); i++)
	{
		std::string ukaz = ((cc + "-c ") + kernel_c[i]) + " -o ";
		std::string izhodna_dat = kernel_c[i];
		izhodna_dat.pop_back();
		izhodna_dat += "o";
		std::cout<<kernel_c[i]<<" > "<<izhodna_dat<<"\n";
		ukaz += izhodna_dat;
		std::system(ukaz.c_str());
		odvecne_dat.push_back(izhodna_dat);
	}
}

void multiboot()
{
	std::string ukaz = s + grub_s + " -o ";
	std::string izhodna_dat = grub_s;
	izhodna_dat.pop_back();
	izhodna_dat += "o";
	ukaz += izhodna_dat;
	std::cout<<grub_s<<" > "<<izhodna_dat<<"\n";
	std::system(ukaz.c_str());
	odvecne_dat.push_back(izhodna_dat);
}

void link_grub()
{
	std::string izhodna_dat = projekt + "_grub.bin";
	std::string ukaz = ld + " -T " + linker_ld + " -o " + izhodna_dat + " ";
	
	for(int i = 0;i < kernel_c.size(); i++)
	{
		ukaz += kernel_c[i];
		ukaz.pop_back();
		ukaz += "o ";	
	}
	ukaz += grub_s;
	ukaz.pop_back();
	ukaz += "o";
	std::cout<<linker_ld<<" > "<<izhodna_dat<<"\n";
	std::system(ukaz.c_str());
}

void pocisti()
{
	for(int i=0;i<odvecne_dat.size();i++)
	{
		std::remove(odvecne_dat[i].c_str());
		std::cout<<"X "<<odvecne_dat[i]<<"\n";
	}
}

int main()
{
	naredi_kernel();
	multiboot();
	link_grub();
	pocisti();
}

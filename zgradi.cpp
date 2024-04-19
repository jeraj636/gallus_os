#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> kernel_c = 
{
	"src/kernel/kernel.c",
	"src/kernel/preprost_vga_gonilnik/preprost_vga_gonilnik.c"
};
std::vector<std::string> pocisti;

std::string CC = "i686-elf-gcc -std=gnu99 -ffreestanding -O2 -Wall -Wextra";
std::string S = "i686-elf-as ";
std::string LD = "i686-elf-gcc -ffreestanding -O2 -nostdlib -lgcc ";
std::string LINKER = "src/grub_zagon/grub_linker.ld";
std::string GRUB = "src/grub_zagon/grub.s";
std::string PROJEKT = "gallus_os";
int main()
{
	for(int i=0;i<kernel_c.size();i++)
	{
		std::string ukaz = CC;
		ukaz +=" -c ";
		ukaz +=kernel_c[i];
		ukaz +=" -o ";
		ukaz +=kernel_c[i];
		ukaz.pop_back();
		ukaz.push_back('o');
		std::cout<<kernel_c[i]<<"\n";
		pocisti.push_back(kernel_c[i]);
		pocisti.back().pop_back();
		pocisti.back().push_back('o');
		std::system(ukaz.c_str());
	}	
	std::cout<<"grub\n";
	
	std::string ukaz = S;
	ukaz += GRUB;
	ukaz += " -o ";
	ukaz += GRUB;
	ukaz.pop_back();
	ukaz.push_back('o');
	pocisti.push_back(ukaz);
	std::cout<<ukaz<<"\n";	
	std::system(ukaz.c_str());
	
	ukaz = LD;
	ukaz += "-T ";
	ukaz += LINKER;
	ukaz += " -o ";
	ukaz += PROJEKT;
	ukaz += "_grub.bin ";
	for(int i=0; i < kernel_c.size();i++)
	{
		ukaz += kernel_c[i];
		ukaz.pop_back();
		ukaz+="o ";
	}
	ukaz += GRUB;
	ukaz.pop_back();
	ukaz += "o";
	std::cout<<ukaz<<"\n";
	std::system(ukaz.c_str());

}

#include "../h/Menu.h"



int main(int argc, char** argv)
{
    //C:\Users\olgam\Desktop\bmp\POOP_1\\POOP_1
    Image image = *Image::createInstance();
    Menu* menu = new Menu();
    while (menu->is_the_end() == false)
    {
        menu->print_menu();
        try {
            menu->read_menu_option();
            Menu* new_menu = menu->execute_option(image, std::cin);
            if (new_menu)
            {
                delete menu;
                menu = new_menu;
            }
        }
        catch (InvalidMenuOption& e) { std::cout << e.what(); }
        catch (InvalidInput & e) { std::cout << e.what(); }
    }
   
    return true;
}
  
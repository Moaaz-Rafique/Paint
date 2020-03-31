#include <SFML/Graphics.hpp>
#include<iostream>
#include<vector>
#include<string>
#include<conio.h>

using namespace sf;
using namespace std;
RenderWindow rWindow(sf::VideoMode(1280, 1024), "PAINT");// The rWindow everything is rendered to
CircleShape pen((1));

RenderTexture rTexture;



int selected = 1;
int shape_selected = 0;

RectangleShape screenimage;

sf::Color foreground = Color::Black;
sf::Color background = Color::White;

Texture textureforimage;


sf::RectangleShape pixel(sf::Vector2f(1, 1));



int number = 0;

void floodFill(signed int x, signed int y, sf::Color oldcolor, sf::Color newcolor)
{


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))exit(0);

	if (rTexture.getTexture().copyToImage().getPixel(x, y) == newcolor)
	{
		return;
	}
	else if (rTexture.getTexture().copyToImage().getPixel(x, y) == oldcolor)
	{
		//pixel.setOrigin(pixel.getRadius(),pixel.getRadius());
		pixel.setFillColor(newcolor);
		pixel.setPosition(x, y);
		rTexture.draw(pixel);
		rTexture.display();
		pixel.setPosition(x + (rWindow.getSize().x / 4), y + (rWindow.getSize().y / 4));
		rWindow.draw(pixel);
		rWindow.display();
		if (rTexture.getTexture().copyToImage().getPixel(x + 1, y) == oldcolor)floodFill(x + 1, y, oldcolor, newcolor);
		if (rTexture.getTexture().copyToImage().getPixel(x, y + 1) == oldcolor)floodFill(x, y + 1, oldcolor, newcolor);
		if (rTexture.getTexture().copyToImage().getPixel(x - 1, y) == oldcolor)floodFill(x - 1, y, oldcolor, newcolor);
		if (rTexture.getTexture().copyToImage().getPixel(x, y - 1) == oldcolor)floodFill(x, y - 1, oldcolor, newcolor);

	}


	return;
}



int main()
{
	//colour wheel
	

	//save buttons
	String savebuttonsname[2] = { "src\\save.png","src\\load.png" };
	Texture textureforsavebuttons[2];
	for (int i = 0; i < 2; i++)
		textureforsavebuttons[i].loadFromFile(savebuttonsname[i]);
	sf::RectangleShape savebuttons[2];
	for (int i = 0; i < 2; i++)
	{
		savebuttons[i].setSize(Vector2f(100, 80));

		savebuttons[i].setTexture(&textureforsavebuttons[i]);

		savebuttons[i].setPosition(100, rWindow.getSize().y / 4 + 250+100 * i);
	}

	String undobuttonsname = "src\\undo.png";
	Texture textureforundobuttons;
	textureforundobuttons.loadFromFile(undobuttonsname);
	sf::RectangleShape undobutton;
	
	undobutton.setSize(Vector2f(100, 80));
	undobutton.setTexture(&textureforundobuttons);
	undobutton.setPosition(500, rWindow.getSize().y / 4 - 100);
	undobutton.setScale(-1, 1);


	//icons

	String buttonsname[5] = { "src\\eraser.png","src\\2.png","src\\line.png","src\\c.png","src\\s.png" };
	Texture textureforbuttons[5];
	for (int i = 0; i < 5; i++)
		textureforbuttons[i].loadFromFile(buttonsname[i]);
	sf::RectangleShape buttons[5];
	for (int i = 0; i < 5; i++)
	{
		buttons[i].setSize(Vector2f(50, 50));

		buttons[i].setTexture(&textureforbuttons[i]);

		buttons[i].setPosition(1150, rWindow.getSize().y / 4 + 100 * i);
	}
	String colour = { "src\\wheel.png" };
	Texture colour1;
	colour1.loadFromFile((colour));
	sf::CircleShape colourbox;
	colourbox.setRadius(100);
	colourbox.setPosition(50, 250);
	colourbox.setTexture(&colour1);
	

	int state = 0;
	int max_undo = 0;
	string loc[50];// = { "dataforundo\\test1.png" , "dataforundo\\test2.png" , "dataforundo\\test3.png" , "dataforundo\\test4.png" , "dataforundo\\test5.png" };
	
	

	rTexture.create(640, 480);
	rTexture.clear(sf::Color::White);

	for (int i = 0; i < 50; i++)
	{
		loc[i] = "dataforundo\\test" + to_string(i) + ".png";
		rTexture.getTexture().copyToImage().saveToFile(loc[i]);
	}

	textureforimage.loadFromFile(loc[4]);

	screenimage.setSize(Vector2f(640, 480));
	//screenimage.setOrigin(Vector2f(screenimage.getSize().x , screenimage.getSize().y));
	screenimage.setPosition(Vector2f(rWindow.getSize().x / 4, rWindow.getSize().y / 4));
	screenimage.setTexture(&textureforimage);

	std::vector<sf::VertexArray> vertices; // vector in wich all vertexArrays will be stored
	vertices.push_back(sf::VertexArray()); // The 1. Line
	vertices[0].setPrimitiveType(sf::LinesStrip);
	rTexture.create(640, 480);
	int lines_number = 0; // The index of the current_line
	int locked = false; // When a mouse button is pressed this will change to true until a mouse button is released again



	sf::Vector2i last_Mouse_pos(0, 0);

	RectangleShape eraser;
	eraser.setFillColor(background);
	eraser.setSize(Vector2f(5, 5));

	RectangleShape rectangle;
	rectangle.setFillColor(background);
	rectangle.setSize(Vector2f(0, 0));
	rectangle.setOutlineColor(foreground);
	rectangle.setOutlineThickness(1);
	CircleShape circle;
	circle.setFillColor(background);
	circle.setRadius(0);
	circle.setOutlineColor(foreground);
	circle.setOutlineThickness(1);
	rWindow.setFramerateLimit(60);//frame limit

	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f(0, 0)),
		sf::Vertex(sf::Vector2f(0, 0))
	};

	sf::Vector2f Border_Offset(-5, -25); // Compensate for the Window frame when calling rWindow.getPosition()
	rWindow.clear(sf::Color::Cyan); //Clear the rWindow with a specific color
	sf::Font font;
	if (!font.loadFromFile("src\\font.ttf"))
		return EXIT_FAILURE;
	sf::Text shortcut;
	shortcut.setFont(font);
	shortcut.setCharacterSize(20);
	shortcut.setPosition(1100, rWindow.getSize().y-150);
	shortcut.setString("Press \'F\' for Fill\nPress \'Z\' for Undo\nPress \'Y\' for Redo");
	shortcut.setFillColor(sf::Color::Black);
	sf::Event event;
	while(rWindow.isOpen())
	{		
		// Event processing
		sf::Event event;
		while (rWindow.pollEvent(event))
		{

			if (event.type == sf::Event::KeyPressed) // Handling the closure of the renderrWindow
				if (event.key.code == sf::Keyboard::Key::Escape)
				{
					
					rWindow.close();
					rTexture.getTexture().copyToImage().saveToFile("result\\final.png");										
				}
			if (event.type == sf::Event::Closed) // Handling the closure of the renderrWindow
			{
				rWindow.close();
				rTexture.getTexture().copyToImage().saveToFile("result\\final.png");
			}

			
			//rTexture.getTexture().copyToImage().saveToFile(loc[state%50]);
			//rWindow.clear(Color::White);
			if (sf::Mouse::isButtonPressed(Mouse::Left) && Mouse::getPosition(rWindow).x > 1150
				&& Mouse::getPosition(rWindow).y > rWindow.getSize().y / 4 && Mouse::getPosition(rWindow).x < 1150 + 50
				&& Mouse::getPosition(rWindow).y < rWindow.getSize().y / 4 + 50);
			//------------------------------------undo mechanism-------------------------------------------------

			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) || (sf::Mouse::isButtonPressed(sf::Mouse::Left) && Mouse::getPosition(rWindow).x > 700
				&& Mouse::getPosition(rWindow).y > rWindow.getSize().y / 4 - 100 && Mouse::getPosition(rWindow).x < 700 + 100
				&& Mouse::getPosition(rWindow).y < rWindow.getSize().y / 4 + 20))
				if (state > 0)
				{
					state--;
					textureforimage.loadFromFile(loc[state%50]);
					rTexture.clear(sf::Color::White);
					screenimage.setPosition(Vector2f(0, 0));
					rTexture.draw(screenimage);
					screenimage.setPosition(Vector2f(rWindow.getSize().x / 4, rWindow.getSize().y / 4));
					rWindow.draw(screenimage);
					rTexture.display();
					rWindow.display();
				}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y) || (sf::Mouse::isButtonPressed(sf::Mouse::Left) && Mouse::getPosition(rWindow).x > 500-100
				&& Mouse::getPosition(rWindow).y > rWindow.getSize().y / 4 - 100 && Mouse::getPosition(rWindow).x < 500
				&& Mouse::getPosition(rWindow).y < rWindow.getSize().y / 4 + 20))
				if (state < max_undo)
				{
					state++;
					textureforimage.loadFromFile(loc[state%50]);
					rTexture.clear(sf::Color::White);
					//rWindow.clear(sf::Color::White);
					screenimage.setPosition(Vector2f(0, 0));
					rTexture.draw(screenimage);
					screenimage.setPosition(Vector2f(rWindow.getSize().x / 4, rWindow.getSize().y / 4));
					rWindow.draw(screenimage);

					rTexture.display();

					rWindow.display();
				}
			//------------------------------------------COLOR WHEEL PICK-UP-------------------------------------------------\\

			float r = sqrt(((colourbox.getPosition().x + 100) - Mouse::getPosition(rWindow).x) * ((colourbox.getPosition().x + 100) - Mouse::getPosition(rWindow).x)
						 + ((colourbox.getPosition().y + 100) - Mouse::getPosition(rWindow).y) * ((colourbox.getPosition().y + 100) - Mouse::getPosition(rWindow).y));
			if (event.type == sf::Event::MouseButtonPressed && r<100)
			{
				if ((sf::Mouse::isButtonPressed(sf::Mouse::Left)))
				{	
					foreground = colour1.copyToImage().getPixel(Mouse::getPosition(rWindow).x-colourbox.getPosition().x, Mouse::getPosition(rWindow).y-colourbox.getPosition().y);
					circle.setOutlineColor(foreground);				
					rectangle.setOutlineColor(foreground);
				}
				if ((sf::Mouse::isButtonPressed(sf::Mouse::Right)))
				{
					background = colour1.copyToImage().getPixel(Mouse::getPosition(rWindow).x - colourbox.getPosition().x, Mouse::getPosition(rWindow).y - colourbox.getPosition().y);
					circle.setFillColor(background);
					rectangle.setFillColor(background);
				}
			}
			//------------------------------------Pen select-------------------------------------------------
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::P) || (event.type == sf::Event::MouseButtonPressed && Mouse::getPosition(rWindow).x > 1150
				&& Mouse::getPosition(rWindow).y > rWindow.getSize().y / 4 + 100 * 4 && Mouse::getPosition(rWindow).x < 1150 + 50
				&& Mouse::getPosition(rWindow).y < rWindow.getSize().y / 4 + 450))selected = 0;
			//------------------------------------Box select-------------------------------------------------
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::B) || (event.type == sf::Event::MouseButtonPressed && Mouse::getPosition(rWindow).x > 1150
				&& Mouse::getPosition(rWindow).y > rWindow.getSize().y / 4 + 100 && Mouse::getPosition(rWindow).x < 1150 + 50
				&& Mouse::getPosition(rWindow).y < rWindow.getSize().y / 4 + 150))selected = 1;
			//------------------------------------Line Select-------------------------------------------------
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::L) || (event.type == sf::Event::MouseButtonPressed && Mouse::getPosition(rWindow).x > 1150
				&& Mouse::getPosition(rWindow).y > rWindow.getSize().y / 4 + 100 * 2 && Mouse::getPosition(rWindow).x < 1150 + 50
				&& Mouse::getPosition(rWindow).y < rWindow.getSize().y / 4 + 250))selected = 3;
			//------------------------------------Circle select-------------------------------------------------
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::C) || (event.type == sf::Event::MouseButtonPressed && Mouse::getPosition(rWindow).x > 1150
				&& Mouse::getPosition(rWindow).y > rWindow.getSize().y / 4 + 100 * 3 && Mouse::getPosition(rWindow).x < 1150 + 50
				&& Mouse::getPosition(rWindow).y < rWindow.getSize().y / 4 + 350))selected = 2;	
			//------------------------------------Pen select-------------------------------------------------
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) || (event.type == sf::Event::MouseButtonPressed && Mouse::getPosition(rWindow).x > 1150
				&& Mouse::getPosition(rWindow).y > rWindow.getSize().y / 4   && Mouse::getPosition(rWindow).x < 1150 + 50
				&& Mouse::getPosition(rWindow).y < rWindow.getSize().y / 4 + 100))selected = 4;
			//------------------------------------flood fill-------------------------------------------------
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
			{

				int col1 = 1;
				sf::Color colorselected = rTexture.getTexture().copyToImage().
					getPixel(Mouse::getPosition(rWindow).x - (rWindow.getSize().x / 4),
						Mouse::getPosition(rWindow).y - (rWindow.getSize().y / 4));
				int col2 = 2;
				pixel.setFillColor(foreground);
				float x = Mouse::getPosition(rWindow).x - (rWindow.getSize().x / 4);
				float y = Mouse::getPosition(rWindow).y - (rWindow.getSize().y / 4);
				floodFill(x, y, colorselected, foreground);
				if (state < 49)
				{
					state++;
					max_undo++;
				}
				else
				{
					rTexture.getTexture().copyToImage().saveToFile(loc[state%50]);
					for (int i = 1; i < 50; i++)
					{
						textureforimage.loadFromFile(loc[i]);
						screenimage.setTexture(&textureforimage);
						screenimage.setPosition(Vector2f(0, 0));
						rTexture.draw(screenimage);
						rTexture.display();
						rTexture.getTexture().copyToImage().saveToFile(loc[i - 1]);
					}
					screenimage.setPosition(Vector2f(rWindow.getSize().x / 4, rWindow.getSize().y / 4));
				}

			}

			
			///------------------------------------save the image------------------------
			
			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::S)) || (event.type == sf::Event::MouseButtonPressed  && Mouse::getPosition(rWindow).x > 100
				&& Mouse::getPosition(rWindow).y > rWindow.getSize().y / 4 + 250 && Mouse::getPosition(rWindow).x < 100 + 100
				&& Mouse::getPosition(rWindow).y < rWindow.getSize().y / 4 + 250 + 80))
			{
				bool base = true;
				bool save = true;
				string save_file_name;
				sf::Text save_file;
				save_file.setFont(font);
				save_file.setCharacterSize(20);
				save_file.setFillColor(sf::Color::Black);
				save_file_name.clear();
				while (base)
				{					
					string save_menu = "Type the name of file:\n\n\n\n\nPress Enter to save\n\t and \nEscape to cancel";
					save_file.setPosition(10, 10);
					save_file.setString(save_menu);
					rWindow.draw(save_file);
					save_file.setPosition(10, 50);
					save_file.setString(save_file_name);
					rWindow.draw(save_file);
					rWindow.display();
					cout << sf::Keyboard::Key::Z;
					rWindow.pollEvent(event);
					rWindow.setFramerateLimit(10);
					if (event.type == sf::Event::TextEntered) // Handling the closure of the renderrWindow
						if (event.text.unicode < 128)
						{
							save_file_name += (char)event.text.unicode;
						}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) base = false;
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
					{
						base = false;
						save = false;
					}
				}
				rWindow.setFramerateLimit(60);
				if (save)rTexture.getTexture().copyToImage().saveToFile("savefiles\\" + save_file_name + ".png");				
			}

			///------------------------------------load the image------------------------

			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::L)) || (event.type == sf::Event::MouseButtonPressed && Mouse::getPosition(rWindow).x > 100
				&& Mouse::getPosition(rWindow).y > rWindow.getSize().y / 4 + 350 && Mouse::getPosition(rWindow).x < 100 + 100
				&& Mouse::getPosition(rWindow).y < rWindow.getSize().y / 4 + 350 + 80))
			{
				bool base = true;
				bool load = true;
				string load_file_name;
				sf::Text load_file;
				load_file.setFont(font);
				load_file.setCharacterSize(20);
				load_file.setFillColor(sf::Color::Black);
				load_file_name.clear();
				while (base)
				{
					string save_menu = "Type the name of file:\n\n\n\n\nPress Enter to load\n\t and \nEscape to cancel";
					load_file.setPosition(10, 10);
					load_file.setString(save_menu);
					rWindow.draw(load_file);
					load_file.setPosition(10, 50);
					load_file.setString(load_file_name);
					rWindow.draw(load_file);
					rWindow.display();
					cout << sf::Keyboard::Key::Z;
					rWindow.pollEvent(event);
					rWindow.setFramerateLimit(10);
					if (event.type == sf::Event::TextEntered) // Handling the closure of the renderrWindow
						if (event.text.unicode < 128)
						{
							load_file_name += event.text.unicode;
						}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) base = false;
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
					{
						base = false;
						load = false;
					}
				}
				rWindow.setFramerateLimit(60);
				screenimage.setPosition(Vector2f(0, 0));
				textureforimage.loadFromFile(load_file_name + ".png");
				screenimage.setTexture(&textureforimage);
				rTexture.clear(sf::Color::White);
				rTexture.draw(screenimage); 
				screenimage.setPosition(Vector2f(rWindow.getSize().x / 4, rWindow.getSize().y / 4));
				rWindow.draw(screenimage);

				rWindow.display();
			}			

			if (event.type == sf::Event::MouseButtonPressed && selected == 0) // See "locked" definition
			{
				locked = true;

			}
			///-----------------------------------------MAking the LiNes---------------------------------
			if ((event.type == sf::Event::MouseButtonPressed) && selected == 3)
			{
				line[0]=sf::Vertex(sf::Vector2f(Mouse::getPosition(rWindow).x, Mouse::getPosition(rWindow).y),foreground);
				while (!(event.type == sf::Event::MouseButtonReleased))
				{
					rWindow.pollEvent(event);
					rWindow.clear(sf::Color::Cyan);
					line[1] = sf::Vertex(sf::Vector2f(Mouse::getPosition(rWindow).x, Mouse::getPosition(rWindow).y), foreground);

					rWindow.draw(screenimage);
					rWindow.draw(line, 2, sf::Lines);
					for (int i = 0; i < 2; i++)
					{
						rWindow.draw(savebuttons[i]);
					}
					//icons
					for (int i = 0; i < 5; i++)
					{
						rWindow.draw(buttons[i]);
					}

					//color 
					rWindow.draw(colourbox);
					rWindow.draw(shortcut);
					rWindow.display();
				}
				float x0 = line[0].position.x;
				float y0 = line[0].position.y;
				float x1 = line[1].position.x;
				float y1 = line[1].position.y;				
				line[0] = sf::Vertex(sf::Vector2f(x0 - (rWindow.getSize().x / 4), y0 - (rWindow.getSize().y / 4)), foreground);				
				line[1] = sf::Vertex(sf::Vector2f(x1 - (rWindow.getSize().x / 4), y1 - (rWindow.getSize().y / 4)), foreground);
				rTexture.draw(line, 2, sf::Lines);
				rTexture.display();
				if (state < 49)
				{
					state++;
					max_undo = state;
				}
				else
				{
					rTexture.getTexture().copyToImage().saveToFile(loc[state%50]);
					for (int i = 1; i < 50; i++)
					{
						textureforimage.loadFromFile(loc[i]);
						screenimage.setTexture(&textureforimage);
						screenimage.setPosition(Vector2f(0, 0));
						rTexture.draw(screenimage);
						rTexture.display();
						rTexture.getTexture().copyToImage().saveToFile(loc[i - 1]);
					}
					screenimage.setPosition(Vector2f(rWindow.getSize().x / 4, rWindow.getSize().y / 4));
				}
				
			}
			///-----------------------------------------MAking the BOX---------------------------------
			if ((event.type == sf::Event::MouseButtonPressed) && selected == 1)
			{
				rectangle.setPosition(Mouse::getPosition(rWindow).x, Mouse::getPosition(rWindow).y);
				while (!(event.type == sf::Event::MouseButtonReleased))
				{
					rWindow.pollEvent(event);
					rWindow.clear(sf::Color::Cyan);
					rectangle.setSize(-Vector2f(rectangle.getPosition().x - Mouse::getPosition(rWindow).x,
						rectangle.getPosition().y - Mouse::getPosition(rWindow).y));

					rWindow.draw(screenimage);
					rWindow.draw(rectangle);
					for (int i = 0; i < 2; i++)
					{
						rWindow.draw(savebuttons[i]);
					}
					//icons
					for (int i = 0; i < 5; i++)
					{
						rWindow.draw(buttons[i]);
					}

					//color 
					rWindow.draw(colourbox);
					rWindow.draw(shortcut);
					rWindow.display();
				}
				float x = rectangle.getPosition().x;
				float y = rectangle.getPosition().y;
				rectangle.setPosition(x - (rWindow.getSize().x / 4), y - (rWindow.getSize().y / 4));
				rTexture.draw(rectangle);
				rTexture.display();
				if (state < 49)
				{
					state++;
					max_undo = state;
				}
				else
				{
					rTexture.getTexture().copyToImage().saveToFile(loc[state%50]);
					for (int i = 1; i < max_undo; i++)
					{
						textureforimage.loadFromFile(loc[i]);
						screenimage.setTexture(&textureforimage);
						screenimage.setPosition(Vector2f(0, 0));
						rTexture.draw(screenimage);
						rTexture.display();
						rTexture.getTexture().copyToImage().saveToFile(loc[i-1]);
					}
					screenimage.setPosition(Vector2f(rWindow.getSize().x / 4, rWindow.getSize().y / 4));
				}

			}
			///----------------------------------MAking the CIrcle---------------------------------------
			if ((event.type == sf::Event::MouseButtonPressed) && selected == 2) 
			{
				circle.setPosition(Mouse::getPosition(rWindow).x, Mouse::getPosition(rWindow).y);
				while (!(event.type == sf::Event::MouseButtonReleased))
				{
					rWindow.pollEvent(event);
					rWindow.clear(sf::Color::Cyan);
					circle.setRadius((Mouse::getPosition(rWindow).x - circle.getPosition().x) / 2);

					rWindow.draw(screenimage);
					rWindow.draw(circle);
					for (int i = 0; i < 2; i++)
					{
						rWindow.draw(savebuttons[i]);
					}
					//icons
					for (int i = 0; i < 5; i++)
					{
						rWindow.draw(buttons[i]);
					}

					//color 
					rWindow.draw(colourbox);
					rWindow.draw(shortcut);
					rWindow.display();

				}
				float x = circle.getPosition().x;
				float y = circle.getPosition().y;
				circle.setPosition(x - (rWindow.getSize().x / 4), y - (rWindow.getSize().y / 4));
				rTexture.draw(circle);
				if (state < 49)
				{
					state++;
					max_undo = state;
				}
				else
				{
					rTexture.getTexture().copyToImage().saveToFile(loc[state%50]);
					for (int i = 1; i < 50; i++)
					{
						textureforimage.loadFromFile(loc[i]);
						screenimage.setTexture(&textureforimage);
						screenimage.setPosition(Vector2f(0, 0));
						rTexture.draw(screenimage);
						rTexture.display();
						rTexture.getTexture().copyToImage().saveToFile(loc[i - 1]);
					}
					screenimage.setPosition(Vector2f(rWindow.getSize().x / 4, rWindow.getSize().y / 4));
				}
			}
			///----------------------------------EraSer---------------------------------------
			if ((event.type == sf::Event::MouseButtonPressed) && selected == 4)
			{
				eraser.setFillColor(background);
				while (!(event.type == sf::Event::MouseButtonReleased))
				{
					
					float x = Mouse::getPosition(rWindow).x;
					float y = Mouse::getPosition(rWindow).y;
					rWindow.pollEvent(event);
					eraser.setPosition(x , y);
					rWindow.draw(eraser);
					rWindow.display();					
					eraser.setPosition(x - (rWindow.getSize().x / 4), y - (rWindow.getSize().y / 4));
					rTexture.draw(eraser);
					rTexture.display();
				}
				if (state < 49)
				{
					state++;
					max_undo++;
				}
				else
				{
					rTexture.getTexture().copyToImage().saveToFile(loc[state%50]);
					for (int i = 1; i < 50; i++)
					{
						textureforimage.loadFromFile(loc[i]);
						screenimage.setTexture(&textureforimage);
						screenimage.setPosition(Vector2f(0, 0));
						rTexture.draw(screenimage);
						rTexture.display();
						rTexture.getTexture().copyToImage().saveToFile(loc[i - 1]);
					}
					screenimage.setPosition(Vector2f(rWindow.getSize().x / 4, rWindow.getSize().y / 4));
				}
			}
			
			if (event.type == sf::Event::MouseButtonReleased) // See "locked" definition
			{
				// Add a new Line
				lines_number++;
				vertices.push_back(sf::VertexArray());
				vertices[lines_number].setPrimitiveType(sf::LinesStrip);
				
				if(locked)
					if (state < 49)
					{
						state++;
						max_undo = state;
					}
					else
					{
						rTexture.getTexture().copyToImage().saveToFile(loc[state%50]);
						for (int i = 1; i < 50; i++)
						{
							textureforimage.loadFromFile(loc[i]);
							screenimage.setTexture(&textureforimage);
							screenimage.setPosition(Vector2f(0, 0));
							rTexture.draw(screenimage);
							rTexture.display();
							rTexture.getTexture().copyToImage().saveToFile(loc[i - 1]);
						}
						screenimage.setPosition(Vector2f(rWindow.getSize().x / 4, rWindow.getSize().y / 4));
					}
				locked = false; // Reset			
			}
				
		}

		//rWindow.draw(screenimage);

		if (locked) // See "locked" definition
		{
			if (last_Mouse_pos != sf::Mouse::getPosition()) // When the Mouse hasn't moved don't add any new Vertex (save memory)
			{
				//.append(Position, Color) : .append(MousePos - WindowPos + MouseOffset, curr_col)

				if ((sf::Mouse::getPosition(rWindow).x > rWindow.getSize().x / 4) && (sf::Mouse::getPosition(rWindow).y > rWindow.getSize().y / 4))vertices[lines_number].append(sf::Vertex(sf::Vector2f(sf::Mouse::getPosition(rWindow).x - rWindow.getSize().x / 4, sf::Mouse::getPosition(rWindow).y - (rWindow.getSize().y / 4)), foreground));

				last_Mouse_pos = sf::Mouse::getPosition();
			}
		}
		


		//curr_col = sf::Color::Color(rand() % 255, rand() % 255, rand() % 255);


		//std::cout << "vertices in line " << lines_number << ": " << vertices[lines_number].getVertexCount() << std::endl;



		//Draw everything (vertices)
		rWindow.clear(sf::Color::Cyan);

		screenimage.setPosition(Vector2f(0, 0));
		rTexture.draw(vertices[lines_number]);
		//rWindow.draw(vertices[lines_number]);
		rTexture.display();
		rTexture.getTexture().copyToImage().saveToFile(loc[state%50]);
		textureforimage.loadFromFile(loc[state%50]);
		screenimage.setTexture(&textureforimage);
		rTexture.clear(sf::Color::White);

		rTexture.draw(screenimage);
		screenimage.setPosition(Vector2f(rWindow.getSize().x / 4, rWindow.getSize().y / 4));
		rWindow.draw(screenimage);


		rTexture.display();
		//save
		for (int i = 0; i < 2; i++)
		{
			rWindow.draw(savebuttons[i]);
		}
		//icons
		for (int i = 0; i < 5; i++)
		{
			rWindow.draw(buttons[i]);
		}

		//color 
		rWindow.draw(colourbox);		
		rWindow.draw(shortcut);
		rWindow.display();
	}	
	
	return 0;
}
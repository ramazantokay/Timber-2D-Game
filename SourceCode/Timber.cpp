// Timber.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Created by Ramazan Tokay 

#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
using namespace sf;

//Function declaration 
void updateBranches(int seed);

const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

enum class side { LEFT, RIGHT, NONE };
side branchPositions[NUM_BRANCHES];

int main()
{
	//Create a video mode
	VideoMode vm(1920, 1080);

	//Create a window
	RenderWindow window(vm, "Timber!!!", Style::Titlebar);

	/*Background*/
	Texture textureBackground;
	textureBackground.loadFromFile("graphics/background.png");
	Sprite spriteBackground;
	spriteBackground.setTexture(textureBackground);
	spriteBackground.setPosition(0, 0);
	/* End of Background*/

	/*Tree*/
	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810, 0);
	/*End of Tree*/

	/*Adding other trees*/
	Texture textureTree2;
	textureTree2.loadFromFile("graphics/tree2.png");
	Sprite spriteTree2;
	spriteTree2.setTexture(textureTree2);
	Sprite spriteTree3;
	spriteTree3.setTexture(textureTree2);
	Sprite spriteTree4;
	spriteTree4.setTexture(textureTree2);
	Sprite spriteTree5;
	spriteTree5.setTexture(textureTree2);
	Sprite spriteTree6;
	spriteTree6.setTexture(textureTree2);

	spriteTree2.setPosition(20, 0);
	spriteTree3.setPosition(300, -400);
	spriteTree4.setPosition(1300, -400);
	spriteTree5.setPosition(1500, -500);
	spriteTree6.setPosition(1900, 0);
	/*End of the adding other trees*/

	
	/*Bee*/
	Texture textureBee;
	textureBee.loadFromFile("graphics/insect.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 800);

	bool beeActive = false;
	float beeSpeed = 0.0f;
	/*End of Bee*/


	/*Cloud*/
	Texture textureCloud;
	textureCloud.loadFromFile("graphics/cloud.png");
	const int MAX_CLOUDS = 6;
	Sprite clouds[MAX_CLOUDS];
	int cloudSpeeds[MAX_CLOUDS];
	bool cloudActive[MAX_CLOUDS];

	for (int i = 0; i<MAX_CLOUDS; i++)
	{
		clouds[i].setTexture(textureCloud);
		clouds[i].setPosition(-300, i * 150);
		cloudActive[i] = false;
		cloudSpeeds[i] = 0;
	}

	Clock clock;

	/*Time bar*/
	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;
		
	bool paused = true;
	/*End of Time bar*/

	/*Font Setting*/
	int score = 0;
	Text messageText, scoreText, fpsText;

	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	messageText.setFont(font);
	scoreText.setFont(font);
	fpsText.setFont(font);

	//Set up fps text
	fpsText.setFillColor(Color::White);
	fpsText.setCharacterSize(100);
	fpsText.setPosition(1200, 20);

	messageText.setString("Press Enter to start!");
	scoreText.setString("Score = 0");

	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);

	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);

	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
	scoreText.setPosition(20, 20);

	/*Rectangle for text background*/
	RectangleShape rect1;
	rect1.setFillColor(Color(0, 0, 0, 150));
	rect1.setSize(Vector2f(600, 105));
	rect1.setPosition(0, 30);
	
	RectangleShape rect2;
	rect2.setFillColor(Color(0, 0, 0, 150));
	rect2.setSize(Vector2f(600, 105));
	rect2.setPosition(1150, 30);
	/*End Font Setting*/

	/*Branches*/
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");
	for (int i = 0; i < NUM_BRANCHES; i++)
	{
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);
		branches[i].setOrigin(220, 20);
	}
	/*End ofBranches*/

	//Prepare the player
	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580, 720);

	//The player starts on the left
	side playerSide = side::LEFT;

	//Prepare gravestone
	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(600, 2000);

	//Preparing the axe
	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(700, 830);

	//Line the axe up with the tree
	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;

	//Prepare the flying log
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720);

	//some other useful log related variables
	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	//control the player input
	bool acceptInput = false;

	//Sound Settings
	SoundBuffer chopbuffer;
	chopbuffer.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopbuffer);

	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	Sound death;
	death.setBuffer(deathBuffer);

	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("sound/out_of_time.wav");
	Sound outOfTime;
	outOfTime.setBuffer(ootBuffer);
	/*End of sound settings*/

	int lastDrawn = 0;

	while (window.isOpen())
	{

		//Handle the players input

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type ==Event::KeyReleased && !paused)
			{
				acceptInput = true;
				spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			paused = false;
			score = 0;
			timeRemaining = 5;

			//Make all branches disappear
			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				branchPositions[i] = side::NONE;
			}
			//Make sure the gravestone is hidden
			spriteRIP.setPosition(675, 2000);

			//Move the player into position
			spritePlayer.setPosition(580, 720);
			acceptInput = true;

		}

		if (acceptInput)
		{
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				playerSide = side::RIGHT;
				score++;

				timeRemaining += (2 / score) + .15;

				spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
				spritePlayer.setPosition(1200, 720);
				updateBranches(score);

				spriteLog.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;
				acceptInput = false;
				//Chop sound play
				chop.play();
			}

			else if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				playerSide = side::LEFT;
				score++;

				timeRemaining += (2 / score) + .15;

				spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
				spritePlayer.setPosition(580, 720);
				updateBranches(score);

				spriteLog.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;
				acceptInput = false;
				chop.play();

			}

		}
		
		//Update the scene
			   		 
		if (!paused)
		{

			Time dt = clock.restart();

			timeRemaining -= dt.asSeconds();
			timeBar.setSize(Vector2f(timeBarWidthPerSecond*timeRemaining, timeBarHeight));

			if (timeRemaining < 0.0f)
			{
				paused = true;

				messageText.setString("Out of time!!");

				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
				outOfTime.play();
			}

			// manage bee
			if (!beeActive)
			{
				srand((int)time(0) * 10);
				beeSpeed = (rand() % 200) + 200;

				srand((int)time(0) * 10);
				float height = (rand() % 500) + 500;
				spriteBee.setPosition(2000, height);
				beeActive = true;

			}
			else
			{
				spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed*dt.asSeconds()), spriteBee.getPosition().y);
				if (spriteBee.getPosition().x < -100)
				{
					beeActive = false;
				}
			}

			//manage clouds
			//cloud 1

			for (int i =0; i<MAX_CLOUDS;i++)
			{
				if (!cloudActive[i])
				{
					srand((int)time(0)*i);
					cloudSpeeds[i] = (rand() % 200);

					srand((int)time(0)*i);
					float height = (rand() % 150);
					clouds[i].setPosition(-200, height);
					cloudActive[i] = true;


				}
				else
				{
					clouds[i].setPosition(clouds[i].getPosition().x + (cloudSpeeds[i] * dt.asSeconds()), clouds[i].getPosition().y);
					if (clouds[i].getPosition().x > 1920)
					{
						cloudActive[i] = false;
					}
				}
						
			}
		
			//Update the score text
			lastDrawn++;
			if (lastDrawn==100)
			{
			std::stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str());
			
			std::stringstream ss2;
			ss2 << "FPS = " << 1 / dt.asSeconds();
			fpsText.setString(ss2.str());
			lastDrawn = 0;

			}

			//update the branch sprites

			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				float height = i * 150;
				if (branchPositions[i] == side::LEFT)
				{
					branches[i].setPosition(610, height);
					branches[i].setRotation(180);
				}
				else if (branchPositions[i] == side::RIGHT)
				{
					branches[i].setPosition(1330, height);
					branches[i].setRotation(0);

				}
				else
				{
					branches[i].setPosition(3000, height);
				}

			}

			if (logActive)
			{
				spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX*dt.asSeconds()), spriteLog.getPosition().y + (logSpeedY*dt.asSeconds()));

				if (spriteLog.getPosition().x<-100 || spriteLog.getPosition().x > 2000)
				{
					logActive = false;
					spriteLog.setPosition(810, 720);
				}
			}

			if (branchPositions[5]==playerSide)
			{
				paused = true;
				acceptInput = false;

				spriteRIP.setPosition(525, 760);
				
				spritePlayer.setPosition(2000, 660);

				messageText.setString("SQUISHED");

				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top+textRect.height/2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

				death.play();
			}	
		}
	 
		// Draw the scene ////////////

		//clear everything from last frame
		window.clear();

		//draw our game scene here
		window.draw(spriteBackground);

		for (int i =0; i<MAX_CLOUDS;i++)
		{
			window.draw(clouds[i]);
		}
		window.draw(spriteTree2);
		window.draw(spriteTree3);
		window.draw(spriteTree4);
		window.draw(spriteTree5);
		window.draw(spriteTree6);

		for (int i = 0; i < NUM_BRANCHES; i++)
		{
			window.draw(branches[i]);
		}

		window.draw(spriteTree);

		window.draw(spritePlayer);

		window.draw(spriteAxe);

		window.draw(spriteLog);

		window.draw(spriteRIP);

		window.draw(rect1);

		window.draw(rect2);

		window.draw(spriteBee);

		window.draw(scoreText);

		window.draw(fpsText);

		window.draw(timeBar);

		if (paused)
		{
			window.draw(messageText);
		}
	
		//show everything we just drew
		window.display();
			   		 	  
	}
	return 0;
}

void updateBranches(int seed)
{
	for (int j = NUM_BRANCHES - 1; j > 0; j--)
	{
		branchPositions[j] = branchPositions[j - 1];
	}

	srand((int)time(0) + seed);
	int r = (rand() % 5);
	switch (r)
	{
	case 0:
		branchPositions[0] = side::LEFT;
		break;
	case 1:
		branchPositions[0] = side::RIGHT;
		break;
	default:
		branchPositions[0] = side::NONE;
		break;

	}
}
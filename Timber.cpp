#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

void updateBranches(int seed);

const int NUM_BRANCHES = 6;
sf::Sprite branches[NUM_BRANCHES];
enum class side
{
    LEFT, RIGHT, NONE
};
side branchPosition[NUM_BRANCHES];

int main()
{
    sf::VideoMode vm(1920, 1080);
    sf::RenderWindow window(vm, "Tinder", sf::Style::Fullscreen);

    sf::Texture textureBackground;
    textureBackground.loadFromFile("graphics/background.png");

    sf::Sprite spriteBackground;
    spriteBackground.setTexture(textureBackground);
    spriteBackground.setPosition(0, 0);

    sf::Texture textureTree;
    textureTree.loadFromFile("graphics/tree.png");

    sf::Sprite spriteTree;
    spriteTree.setTexture(textureTree);
    spriteTree.setPosition(810, 0);

    sf::Texture textureBee;
    textureBee.loadFromFile("graphics/bee.png");

    sf::Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(0, 800);

    bool isBeeFlying{ false };
    float beeSpeed{ 0.0f };

    sf::Texture textureCloud;
    textureCloud.loadFromFile("graphics/cloud.png");

    sf::Sprite spriteCloud1;
    sf::Sprite spriteCloud2;
    sf::Sprite spriteCloud3;

    spriteCloud1.setTexture(textureCloud);
    spriteCloud2.setTexture(textureCloud);
    spriteCloud3.setTexture(textureCloud);

    spriteCloud1.setPosition(0, 0);
    spriteCloud2.setPosition(0, 250);
    spriteCloud3.setPosition(0, 500);

    bool cloud1Active = false;
    bool cloud2Active = false;
    bool cloud3Active = false;

    float cloud1Speed = 0.0f;
    float cloud2Speed = 0.0f;
    float cloud3Speed = 0.0f;

    sf::Clock clock;

    sf::RectangleShape timeBar;
    float timeBarStartWidth = 200.f;
    float timeBarHeight = 80.f;
    timeBar.setSize(sf::Vector2f(timeBarStartWidth, timeBarHeight));
    timeBar.setFillColor(sf::Color::Red);
    timeBar.setPosition((1920 / 2 ) - timeBarStartWidth / 2, 980);

    sf::Time gameTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

    bool paused = true;

    int score = 0;

    sf::Text messageText;
    sf::Text scoreText;

    sf::Font font;
    font.loadFromFile("fonts/KOMIKAP_.ttf");

    messageText.setFont(font);
    scoreText.setFont(font);

    messageText.setString("Press Enter to start!");
    scoreText.setString("Score = 0");

    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(100);

    messageText.setFillColor(sf::Color::White);
    scoreText.setFillColor(sf::Color::White);

    sf::FloatRect textRect = messageText.getLocalBounds();

    messageText.setOrigin(textRect.left + textRect.width / 2.0f, 
                          textRect.top + textRect.height / 2.0f);

    messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

    scoreText.setPosition(20, 20);

    sf::Texture textureBranch;
    textureBranch.loadFromFile("graphics/branch.png");

    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        branches[i].setTexture(textureBranch);
        branches[i].setPosition(-2000, -2000);
        branches[i].setOrigin(220, 20);
    }

    updateBranches(1);
    updateBranches(2);
    updateBranches(3);
    updateBranches(4);
    updateBranches(5);

    sf::Texture texturePlayer;
    texturePlayer.loadFromFile("graphics/player.png");

    sf::Sprite spritePlayer;
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setPosition(580, 720);

    side playerSide = side::LEFT;
    
    sf::Texture textureRip;
    textureRip.loadFromFile("graphics/rip.png");
    sf::Sprite spriteRip;
    spriteRip.setTexture(textureRip);
    spriteRip.setPosition(600, 860);

    sf::Texture textureAxe;
    textureAxe.loadFromFile("graphics/axe.png");
    sf::Sprite spriteAxe;
    spriteAxe.setTexture(textureAxe);
    spriteAxe.setPosition(700, 830);

    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;

    sf::Texture textureLog;
    textureLog.loadFromFile("graphics/log.png");
    sf::Sprite spriteLog;
    spriteLog.setTexture(textureLog);
    spriteLog.setPosition(810, 720);

    bool logActive = false;
    float logSpeedX = 1000.f;
    float logSpeedY = -1500.f;

    bool acceptInput = false;

    sf::SoundBuffer chopBuffer;
    chopBuffer.loadFromFile("sound/chop.wav");
    sf::Sound chop;
    chop.setBuffer(chopBuffer);

    sf::SoundBuffer deathBuffer;
    deathBuffer.loadFromFile("sound/death.wav");
    sf::Sound death;
    death.setBuffer(deathBuffer);

    sf::SoundBuffer ootBuffer;
    ootBuffer.loadFromFile("sound/out_of_time.wav");
    sf::Sound outOfTime;
    outOfTime.setBuffer(ootBuffer);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::KeyReleased && !paused)
            {
                acceptInput = true;

                spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
        {
            paused = false;
            score = 0;
            timeRemaining = 6.f;

            for (int i = 1; i < NUM_BRANCHES; i++)
            {
                branchPosition[i] = side::NONE;
            }

            spriteRip.setPosition(675, 2000);

            spritePlayer.setPosition(580, 720);

            acceptInput = true;
        }

        if (acceptInput)
        {

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                playerSide = side::RIGHT;
                score++;
                timeRemaining += (1.f / score) + .15f;

                spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);

                spritePlayer.setPosition(1200, 720);
                updateBranches(score);

                spriteLog.setPosition(810, 720);
                logSpeedX = -5000;
                logActive = true;

                acceptInput = false;

                chop.play();
                chop.setVolume(50.f);
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                playerSide = side::LEFT;
                score++;
                timeRemaining += (1.f / score) + .15f;

                spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);

                spritePlayer.setPosition(580, 720);
                updateBranches(score);

                spriteLog.setPosition(810, 720);
                logSpeedX = 5000;
                logActive = true;

                acceptInput = false;

                chop.play();
                chop.setVolume(50.f);
            }
        }

        if(!paused)
        {
        sf::Time dt = clock.restart();

        timeRemaining -= dt.asSeconds();
        timeBar.setSize(sf::Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

        if (timeRemaining < 0.f)
        {
            paused = true;
            messageText.setString("Out of time!");
            sf::FloatRect textRect = messageText.getLocalBounds();
            messageText.setOrigin(textRect.left +
                                  textRect.width / 2.0f,
                                  textRect.top +
                                  textRect.height / 2.0f);
            messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
            outOfTime.play();
            outOfTime.setVolume(35);
        }

        if (!isBeeFlying)
        {
            srand((int)time(0));
            beeSpeed = float(rand() % 200 + 200);

            srand((int)time(0) * 10);
            float height = (rand() % 500) + 500;
            spriteBee.setPosition(2000, height);

            isBeeFlying = true;
        }
        else
        {
            spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * dt.asSeconds())
                , spriteBee.getPosition().y);

            if (spriteBee.getPosition().x < -100)
            {
                isBeeFlying = false;
            }
        }

        if (!cloud1Active)
        {
            srand((int)time(0) * 10);
            cloud1Speed = float(rand() % 200);

            srand((int)time(0) * 10);
            float height = (rand() % 150);
            spriteCloud1.setPosition(-200, height);

            cloud1Active = true;
        }
        else
        {
            spriteCloud1.setPosition(spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()),
                spriteCloud1.getPosition().y);

            if (spriteCloud1.getPosition().x > 1920)
                cloud1Active = false;
        }

        if (!cloud2Active)
        {
            srand((int)time(0) * 20);
            cloud2Speed = float(rand() % 200);

            srand((int)time(0) * 20);
            float height = (rand() % 300) - 150;
            spriteCloud2.setPosition(-200, height);

            cloud2Active = true;
        }
        else
        {
            spriteCloud2.setPosition(spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()),
                spriteCloud2.getPosition().y);

            if (spriteCloud2.getPosition().x > 1920)
            {
                cloud2Active = false;
            }

        }

        if (!cloud3Active)
        {
            srand((int)time(0) * 30);
            cloud3Speed = (rand() % 200);

            srand((int)time(0) * 30);
            float height = (rand() % 450) - 150;
            spriteCloud3.setPosition(-200, height);

            cloud3Active = true;
        }
        else
        {
            spriteCloud3.setPosition(spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()),
                spriteCloud3.getPosition().y);

            if (spriteCloud3.getPosition().x > 1920)
            {
                cloud3Active = false;
            }

        }
        std::stringstream ss;
        ss << "Score = " << score;
        scoreText.setString(ss.str());

        for (int i = 0; i < NUM_BRANCHES; i++)
        {
            float height = i * 150.f;

            if (branchPosition[i] == side::LEFT)
            {
                branches[i].setPosition(610, height);
                branches[i].setRotation(180);
            }

            else if (branchPosition[i] == side::RIGHT)
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
            spriteLog.setPosition(spriteLog.getPosition().x + 
                                  (logSpeedX * dt.asSeconds()),
                                  spriteLog.getPosition().y +
                                  (logSpeedY * dt.asSeconds()));

            if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000)
            {
                logActive = false;
                spriteLog.setPosition(810, 720);
            }
        }

        if (branchPosition[5] == playerSide)
        {
            paused = true;
            acceptInput = false;

            spriteRip.setPosition(525, 760);

            spritePlayer.setPosition(2000, 660);

            spriteLog.setPosition(810, 720);

            messageText.setString("SQUISHED!!!!!!");

            sf::FloatRect textRect = messageText.getLocalBounds();

            messageText.setOrigin(textRect.left + textRect.width / 2.f,
                                  textRect.top + textRect.height / 2.f);

            messageText.setPosition(1920 / 2.f, 1080 / 2.f);

            death.play();
            death.setVolume(5);
        }
    }
        window.clear();

        window.draw(spriteBackground);
        
        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteCloud3);

        for (int i = 0; i < NUM_BRANCHES; i++)
        {
            window.draw(branches[i]);
        }

        window.draw(spriteTree);

        window.draw(spritePlayer);

        window.draw(spriteAxe);

        window.draw(spriteLog);

        window.draw(spriteRip);

        window.draw(spriteBee);

        window.draw(scoreText);

        window.draw(timeBar);

        if (paused)
        {
            window.draw(messageText);
        }

        window.display();
    }
}

void updateBranches(int seed)
{
    for (int j = NUM_BRANCHES - 1; j > 0; j--)
    {
        branchPosition[j] = branchPosition[j - 1];
    }
    srand((int)time(0) + seed);
    int r = rand() % 5;
    switch(r)
    {
    case 0:
        branchPosition[0] = side::LEFT;
        break;

    case 1:
        branchPosition[0] = side::RIGHT;
        break;

    default:
        branchPosition[0] = side::NONE;
        break;
    }
}

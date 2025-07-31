#include <iostream>
#include <memory>
#include <vector>
#include <stack>
#include <random>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <functional> // Required for std::function

using namespace std;

// Node structure for linked list
struct CardNode {
    int value;                        // Card value
    bool revealed;                    // Whether the card is revealed
    sf::Sprite sprite;                // Sprite for rendering
    sf::RectangleShape shadow;        // Shadow for elevation effect
    shared_ptr<CardNode> next;        // Pointer to the next node
};

// Linked list class for managing cards
class CardList {
public:
    shared_ptr<CardNode> head;        // Head of the linked list

    // Add a card to the list
    void addCard(int value, const sf::Texture& backTexture) {
        auto newCard = make_shared<CardNode>();
        newCard->value = value;
        newCard->revealed = false;
        newCard->sprite.setTexture(backTexture);
        newCard->shadow.setSize(sf::Vector2f(backTexture.getSize().x, backTexture.getSize().y));
        newCard->shadow.setFillColor(sf::Color(0, 0, 0, 150)); // Semi-transparent black with more opacity
        newCard->next = head;
        head = newCard;
    }

    // Shuffle the linked list
    void shuffle() {
        vector<shared_ptr<CardNode>> nodes;
        for (auto temp = head; temp != nullptr; temp = temp->next) {
            nodes.push_back(temp);
        }

        std::shuffle(nodes.begin(), nodes.end(), default_random_engine(static_cast<unsigned>(time(nullptr))));

        head = nullptr;
        for (auto it = nodes.rbegin(); it != nodes.rend(); ++it) {
            (*it)->next = head;
            head = *it;
        }
    }

    // Traverse and apply a function to each card
    void traverse(function<void(shared_ptr<CardNode>)> func) {
        for (auto temp = head; temp != nullptr; temp = temp->next) {
            func(temp);
        }
    }
};

void setCardPositions(CardList& cardList, sf::RenderWindow& window, int cols, int rows, float spacing) {
    float cardSize = min((window.getSize().x - (cols + 1) * spacing) / cols, (window.getSize().y - (rows + 1) * spacing) / rows) * 0.75f; // Scale down the card size

    // Calculate the offsets to center the grid
    float offsetX = (window.getSize().x - (cols * cardSize + (cols - 1) * spacing)) / 2.f;
    float offsetY = (window.getSize().y - (rows * cardSize + (rows - 1) * spacing)) / 2.f;

    int i = 0;
    cardList.traverse([&](shared_ptr<CardNode> card) {
        int row = i / cols;
        int col = i % cols;

        // Position cards with spacing and offset
        card->sprite.setPosition(
            offsetX + col * (cardSize + spacing),
            offsetY + row * (cardSize + spacing)
        );
        card->sprite.setScale(cardSize / card->sprite.getTexture()->getSize().x, cardSize / card->sprite.getTexture()->getSize().y);

        // Position shadow with more offset from the card
        card->shadow.setPosition(
            card->sprite.getPosition().x + 10.f,
            card->sprite.getPosition().y + 10.f
        );
        card->shadow.setScale(card->sprite.getScale());

        i++;
        });
}

void setupLevel(CardList& cardList, int pairs, const sf::Texture& backTexture) {
    cardList.head = nullptr;
    for (int i = 1; i <= pairs; ++i) {
        for (int j = 0; j < 2; ++j) {  // Two cards per value
            cardList.addCard(i, backTexture);
        }
    }
    cardList.shuffle();
}

int main() {
    // SFML setup
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktop, "Memory Match Cards", sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    // Load textures
    sf::Texture backTexture;
    if (!backTexture.loadFromFile("C:/Uni/3rd/Data Structures project/Project/Project/assets/cards/back.png")) {
        cerr << "Error loading back texture" << endl;
        return -1;
    }

    vector<sf::Texture> cardTextures(12);
    for (int i = 1; i <= 12; ++i) {
        if (!cardTextures[i - 1].loadFromFile("C:/Uni/3rd/Data Structures project/Project/Project/assets/cards/" + to_string(i) + ".png")) {
            cerr << "Error loading card texture " << i << endl;
            return -1;
        }
    }

    // Load background textures
    sf::Texture backgroundTexture1;
    if (!backgroundTexture1.loadFromFile("C:/Uni/3rd/Data Structures project/Project/test4.jpg")) {
        cerr << "Error loading background texture 1" << endl;
        return -1;
    }
    sf::Sprite backgroundSprite1(backgroundTexture1);
    backgroundSprite1.setScale(
        static_cast<float>(window.getSize().x) / backgroundTexture1.getSize().x,
        static_cast<float>(window.getSize().y) / backgroundTexture1.getSize().y
    );

    sf::Texture backgroundTexture2;
    if (!backgroundTexture2.loadFromFile("C:/Uni/3rd/Data Structures project/Project/test2.jpg")) {
        cerr << "Error loading background texture 2" << endl;
        return -1;
    }
    sf::Sprite backgroundSprite2(backgroundTexture2);
    backgroundSprite2.setScale(
        static_cast<float>(window.getSize().x) / backgroundTexture2.getSize().x,
        static_cast<float>(window.getSize().y) / backgroundTexture2.getSize().y
    );

    sf::Texture backgroundTexture3;
    if (!backgroundTexture3.loadFromFile("C:/Uni/3rd/Data Structures project/Project/test3.png")) {
        cerr << "Error loading background texture 3" << endl;
        return -1;
    }
    sf::Sprite backgroundSprite3(backgroundTexture3);
    backgroundSprite3.setScale(
        static_cast<float>(window.getSize().x) / backgroundTexture3.getSize().x,
        static_cast<float>(window.getSize().y) / backgroundTexture3.getSize().y
    );

    sf::Texture backgroundTexture4;
    if (!backgroundTexture4.loadFromFile("C:/Uni/3rd/Data Structures project/Project/test2.jpg")) {
        cerr << "Error loading background texture 4" << endl;
        return -1;
    }
    sf::Sprite backgroundSprite4(backgroundTexture4);
    backgroundSprite4.setScale(
        static_cast<float>(window.getSize().x) / backgroundTexture4.getSize().x,
        static_cast<float>(window.getSize().y) / backgroundTexture4.getSize().y
    );

    // Load font
    sf::Font font;
    if (!font.loadFromFile("C:/Uni/3rd/Data Structures project/Project/Project/assets/arial-font/arial.ttf")) {
        cerr << "Error loading font" << endl;
        return -1;
    }

    // Load sound effects
    sf::SoundBuffer flipBuffer;
    if (!flipBuffer.loadFromFile("C:/Uni/3rd/Data Structures project/Project/Project/assets/sounds/flip.wav")) {
        cerr << "Error loading flip sound" << endl;
        return -1;
    }
    sf::Sound flipSound(flipBuffer);

    sf::SoundBuffer matchBuffer;
    if (!matchBuffer.loadFromFile("C:/Uni/3rd/Data Structures project/Project/Project/assets/sounds/match.wav")) {
        cerr << "Error loading match sound" << endl;
        return -1;
    }
    sf::Sound matchSound(matchBuffer);

    sf::SoundBuffer levelCompleteBuffer;
    if (!levelCompleteBuffer.loadFromFile("C:/Uni/3rd/Data Structures project/Project/Project/assets/sounds/level_complete.wav")) {
        cerr << "Error loading level complete sound" << endl;
        return -1;
    }
    sf::Sound levelCompleteSound(levelCompleteBuffer);

    sf::SoundBuffer nextLevelBuffer;
    if (!nextLevelBuffer.loadFromFile("C:/Uni/3rd/Data Structures project/Project/Project/assets/sounds/next_level.wav")) {
        cerr << "Error loading next level sound" << endl;
        return -1;
    }
    sf::Sound nextLevelSound(nextLevelBuffer);

    // Load background music
    sf::Music backgroundMusic;
    if (!backgroundMusic.openFromFile("C:/Uni/3rd/Data Structures project/Project/Project/assets/sounds/background_music.ogg")) {
        cerr << "Error loading background music" << endl;
        return -1;
    }
    backgroundMusic.setLoop(true);
    backgroundMusic.play();

    // Title text
    sf::Text titleText("Memory Match Cards", font, 100);
    titleText.setFillColor(sf::Color(255, 215, 0)); // Gold
    titleText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    titleText.setOutlineColor(sf::Color::Black);
    titleText.setOutlineThickness(5.f);
    titleText.setPosition(window.getSize().x / 2.f - titleText.getGlobalBounds().width / 2.f, window.getSize().y / 12.f);

    // Title shadow
    sf::Text titleShadow = titleText;
    titleShadow.setFillColor(sf::Color(0, 0, 0, 150)); // Semi-transparent black
    titleShadow.setPosition(titleText.getPosition().x + 5.f, titleText.getPosition().y + 5.f);

    // Play button
    sf::RectangleShape playButton(sf::Vector2f(300.f, 80.f));
    playButton.setFillColor(sf::Color(75, 0, 130)); // Dark purple
    playButton.setOutlineColor(sf::Color::Black);
    playButton.setOutlineThickness(3.f);
    playButton.setPosition(window.getSize().x / 2.f - 150.f, window.getSize().y / 2.5f);

    sf::Text playButtonText("PLAY", font, 50);
    playButtonText.setFillColor(sf::Color::White);
    playButtonText.setStyle(sf::Text::Bold);
    playButtonText.setPosition(
        playButton.getPosition().x + playButton.getSize().x / 2.f - playButtonText.getGlobalBounds().width / 2.f,
        playButton.getPosition().y + playButton.getSize().y / 2.f - playButtonText.getGlobalBounds().height / 2.f
    );

    // Play button shadow
    sf::RectangleShape playButtonShadow = playButton;
    playButtonShadow.setFillColor(sf::Color(0, 0, 0, 150)); // Semi-transparent black
    playButtonShadow.setPosition(playButton.getPosition().x + 5.f, playButton.getPosition().y + 5.f);

    // Exit button
    sf::RectangleShape exitButton(sf::Vector2f(300.f, 80.f));
    exitButton.setFillColor(sf::Color(75, 0, 130)); // Dark purple
    exitButton.setOutlineColor(sf::Color::Black);
    exitButton.setOutlineThickness(3.f);
    exitButton.setPosition(window.getSize().x / 2.f - 150.f, window.getSize().y / 2.5f + 120.f);

    sf::Text exitButtonText("EXIT", font, 50);
    exitButtonText.setFillColor(sf::Color::White);
    exitButtonText.setStyle(sf::Text::Bold);
    exitButtonText.setPosition(
        exitButton.getPosition().x + exitButton.getSize().x / 2.f - exitButtonText.getGlobalBounds().width / 2.f,
        exitButton.getPosition().y + exitButton.getSize().y / 2.f - exitButtonText.getGlobalBounds().height / 2.f
    );

    // Exit button shadow
    sf::RectangleShape exitButtonShadow = exitButton;
    exitButtonShadow.setFillColor(sf::Color(0, 0, 0, 150)); // Semi-transparent black
    exitButtonShadow.setPosition(exitButton.getPosition().x + 5.f, exitButton.getPosition().y + 5.f);

    // Close button for title screen
    sf::RectangleShape closeButtonTitle(sf::Vector2f(30.f, 30.f));
    closeButtonTitle.setFillColor(sf::Color::Red);
    closeButtonTitle.setPosition(window.getSize().x - 40.f, 10.f);

    // Close button for game screen
    sf::RectangleShape closeButtonGame(sf::Vector2f(30.f, 30.f));
    closeButtonGame.setFillColor(sf::Color::Red);
    closeButtonGame.setPosition(window.getSize().x - 40.f, 10.f);

    // Score text
    sf::Text scoreText("Score: 0", font, 40);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setStyle(sf::Text::Bold);
    scoreText.setPosition(window.getSize().x - 200.f, window.getSize().y - 50.f);

    // Score shadow
    sf::Text scoreShadow = scoreText;
    scoreShadow.setFillColor(sf::Color(0, 0, 0, 150)); // Semi-transparent black
    scoreShadow.setPosition(scoreText.getPosition().x + 5.f, scoreText.getPosition().y + 5.f);

    // Match message text
    sf::Text matchMessageText("", font, 40);
    matchMessageText.setFillColor(sf::Color::White);
    matchMessageText.setStyle(sf::Text::Bold);
    matchMessageText.setPosition(20.f, window.getSize().y - 50.f);

    // Match message shadow
    sf::Text matchMessageShadow = matchMessageText;
    matchMessageShadow.setFillColor(sf::Color(0, 0, 0, 150)); // Semi-transparent black
    matchMessageShadow.setPosition(matchMessageText.getPosition().x + 5.f, matchMessageText.getPosition().y + 5.f);

    // Level text
    sf::Text levelText("", font, 60);
    levelText.setFillColor(sf::Color(255, 215, 0)); // Gold
    levelText.setStyle(sf::Text::Bold);
    levelText.setOutlineColor(sf::Color::Black);
    levelText.setOutlineThickness(5.f);
    levelText.setPosition(window.getSize().x / 2.f - levelText.getGlobalBounds().width / 2.f, 20.f);

    // Level shadow
    sf::Text levelShadow = levelText;
    levelShadow.setFillColor(sf::Color(0, 0, 0, 150)); // Semi-transparent black
    levelShadow.setPosition(levelText.getPosition().x + 5.f, levelText.getPosition().y + 5.f);

    // Win message text
    sf::Text winMessageText("Level Completed. SUCCESS!", font, 60);
    winMessageText.setFillColor(sf::Color(255, 215, 0)); // Gold
    winMessageText.setStyle(sf::Text::Bold);
    winMessageText.setOutlineColor(sf::Color::Black);
    winMessageText.setOutlineThickness(5.f);
    winMessageText.setPosition(window.getSize().x / 2.f - winMessageText.getGlobalBounds().width / 2.f, window.getSize().y - 100.f);

    // Win message shadow
    sf::Text winMessageShadow = winMessageText;
    winMessageShadow.setFillColor(sf::Color(0, 0, 0, 150)); // Semi-transparent black
    winMessageShadow.setPosition(winMessageText.getPosition().x + 5.f, winMessageText.getPosition().y + 5.f);

    // Linked list of cards
    CardList cardList;
    bool gameStarted = false;
    bool level1 = false;
    bool level2 = false;
    bool level3 = false;

    // Stack to manage flipped cards
    stack<shared_ptr<CardNode>> flippedCards;
    int matchesFound = 0; // Track the number of matches found
    sf::Clock clock; // Clock to manage the delay
    bool delayActive = false;
    sf::Time delayTime = sf::seconds(0.5); // Delay time for flipping cards back

    // Main game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (!gameStarted) {
                // Handle button clicks on the title screen
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                    if (playButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        gameStarted = true;
                        level1 = true;
                        levelText.setString("LEVEL 1");
                        levelShadow.setString("LEVEL 1");
                        levelText.setPosition(window.getSize().x / 2.f - levelText.getGlobalBounds().width / 2.f, 20.f);
                        levelShadow.setPosition(levelText.getPosition().x + 5.f, levelText.getPosition().y + 5.f);
                        setupLevel(cardList, 4, backTexture); // Level 1 with 4 pairs
                        setCardPositions(cardList, window, 4, 2, 20.f); // 4 columns, 2 rows
                        nextLevelSound.play();
                    }

                    if (exitButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        window.close();
                    }

                    if (closeButtonTitle.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        window.close();
                    }
                }
            }
            else {
                // Handle window resize
                if (event.type == sf::Event::Resized) {
                    if (level1) {
                        setCardPositions(cardList, window, 4, 2, 20.f); // 4 columns, 2 rows
                    }
                    else if (level2) {
                        setCardPositions(cardList, window, 4, 4, 20.f); // 4 columns, 4 rows
                    }
                    else if (level3) {
                        setCardPositions(cardList, window, 6, 4, 20.f); // 6 columns, 4 rows
                    }
                    closeButtonGame.setPosition(window.getSize().x - 40.f, 10.f);
                    if (level1) {
                        backgroundSprite3.setScale(
                            static_cast<float>(window.getSize().x) / backgroundTexture3.getSize().x,
                            static_cast<float>(window.getSize().y) / backgroundTexture3.getSize().y
                        );
                    }
                    else if (level2) {
                        backgroundSprite2.setScale(
                            static_cast<float>(window.getSize().x) / backgroundTexture2.getSize().x,
                            static_cast<float>(window.getSize().y) / backgroundTexture2.getSize().y
                        );
                    }
                    else if (level3) {
                        backgroundSprite4.setScale(
                            static_cast<float>(window.getSize().x) / backgroundTexture4.getSize().x,
                            static_cast<float>(window.getSize().y) / backgroundTexture4.getSize().y
                        );
                    }
                    scoreText.setPosition(window.getSize().x - 200.f, window.getSize().y - 50.f);
                    scoreShadow.setPosition(scoreText.getPosition().x + 5.f, scoreText.getPosition().y + 5.f);
                    matchMessageText.setPosition(20.f, window.getSize().y - 50.f);
                    matchMessageShadow.setPosition(matchMessageText.getPosition().x + 5.f, matchMessageText.getPosition().y + 5.f);
                    levelText.setPosition(window.getSize().x / 2.f - levelText.getGlobalBounds().width / 2.f, 20.f);
                    levelShadow.setPosition(levelText.getPosition().x + 5.f, levelText.getPosition().y + 5.f);
                    winMessageText.setPosition(window.getSize().x / 2.f - winMessageText.getGlobalBounds().width / 2.f, window.getSize().y - 100.f);
                    winMessageShadow.setPosition(winMessageText.getPosition().x + 5.f, winMessageText.getPosition().y + 5.f);
                    }

                    // Mouse click to flip cards
                    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                        if (closeButtonGame.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                            window.close();
                        }

                        cardList.traverse([&](shared_ptr<CardNode> card) {
                            if (!card->revealed && card->sprite.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                                card->revealed = true;
                                card->sprite.setTexture(cardTextures[card->value - 1]);
                                flippedCards.push(card);
                                flipSound.play();

                                if (flippedCards.size() == 2) {
                                    delayActive = true;
                                    clock.restart();
                                }
                            }
                            });
                    }
                }
            }

            if (gameStarted) {
                // Check if delay is active and if the delay time has passed
                if (delayActive && clock.getElapsedTime() >= delayTime) {
                    auto firstCard = flippedCards.top();
                    flippedCards.pop();
                    auto secondCard = flippedCards.top();
                    flippedCards.pop();

                    if (firstCard->value != secondCard->value) {
                        firstCard->revealed = secondCard->revealed = false;
                        firstCard->sprite.setTexture(backTexture);
                        secondCard->sprite.setTexture(backTexture);
                        matchMessageText.setString("No match. Try again.");
                    }
                    else {
                        matchesFound++;
                        matchMessageText.setString("You found a match!");
                        scoreText.setString("Score: " + to_string(matchesFound));
                        matchSound.play();
                        cout << "You found a match! Total matches: " << matchesFound << endl;
                    }

                    delayActive = false;
                }

                // Render the game
                window.clear(sf::Color::White); // Clear with white color
                if (level1) {
                    window.draw(backgroundSprite3);
                }
                else if (level2) {
                    window.draw(backgroundSprite2);
                }
                else if (level3) {
                    window.draw(backgroundSprite4);
                }
                cardList.traverse([&](shared_ptr<CardNode> card) {
                    window.draw(card->shadow); // Draw shadow first
                    window.draw(card->sprite); // Draw card on top of shadow
                    });
                window.draw(closeButtonGame);
                window.draw(scoreShadow);
                window.draw(scoreText);
                window.draw(matchMessageShadow);
                window.draw(matchMessageText);
                window.draw(levelShadow);
                window.draw(levelText);
            }
            else {
                // Render the title screen
                window.clear(sf::Color::Black); // Clear with black color
                window.draw(backgroundSprite1);
                window.draw(titleShadow);
                window.draw(titleText);
                window.draw(playButtonShadow);
                window.draw(playButton);
                window.draw(playButtonText);
                window.draw(exitButtonShadow);
                window.draw(exitButton);
                window.draw(exitButtonText);
                window.draw(closeButtonTitle);
            }

            window.display();

            // Check for game completion
            if (matchesFound == 4 && level1) { // Level 1 has 4 pairs
                cout << "Congratulations! You've completed Level 1!\n";
                levelCompleteSound.play();
                levelText.setString("");
                levelShadow.setString("");
                window.draw(winMessageShadow);
                window.draw(winMessageText);
                window.display();
                sf::sleep(sf::seconds(3));
                level1 = false;
                level2 = true;
                levelText.setString("LEVEL 2");
                levelShadow.setString("LEVEL 2");
                levelText.setPosition(window.getSize().x / 2.f - levelText.getGlobalBounds().width / 2.f, 20.f);
                levelShadow.setPosition(levelText.getPosition().x + 5.f, levelText.getPosition().y + 5.f);
                setupLevel(cardList, 8, backTexture); // Level 2 with 8 pairs
                setCardPositions(cardList, window, 4, 4, 20.f); // 4 columns, 4 rows
                matchesFound = 0;
                scoreText.setString("Score: 0");
                matchMessageText.setString("");
                nextLevelSound.play();
            }

            if (matchesFound == 8 && level2) { // Level 2 has 8 pairs
                cout << "Congratulations! You've completed Level 2!\n";
                levelCompleteSound.play();
                levelText.setString("");
                levelShadow.setString("");
                window.draw(winMessageShadow);
                window.draw(winMessageText);
                window.display();
                sf::sleep(sf::seconds(3));
                level2 = false;
                level3 = true;
                levelText.setString("LEVEL 3");
                levelShadow.setString("LEVEL 3");
                levelText.setPosition(window.getSize().x / 2.f - levelText.getGlobalBounds().width / 2.f, 20.f);
                levelShadow.setPosition(levelText.getPosition().x + 5.f, levelText.getPosition().y + 5.f);
                setupLevel(cardList, 12, backTexture); // Level 3 with 12 pairs
                setCardPositions(cardList, window, 6, 4, 20.f); // 6 columns, 4 rows
                matchesFound = 0;
                scoreText.setString("Score: 0");
                matchMessageText.setString("");
                nextLevelSound.play();
            }

            if (matchesFound == 12 && level3) { // Level 3 has 12 pairs
                cout << "Congratulations! You've completed Level 3!\n";
                levelCompleteSound.play();
                levelText.setString("");
                levelShadow.setString("");
                window.draw(winMessageShadow);
                window.draw(winMessageText);
                window.display();
                sf::sleep(sf::seconds(3));
                window.close();
            }
        }

        return 0;
    }
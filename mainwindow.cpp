#include "mainwindow.h"
#include "./ui_mainwindow.h"

//
#define CREATE 0
#define JOIN 1
#define OFFLINE 2

typedef enum
{
    create,
    join,
    offline,
} PLAYSTATE;


// Global Variables
//
// Player
player player;
// Bot
cpu bot;
// Playstate - semi-statemachine
PLAYSTATE playstate;
//

// Sounds
Sounds menuMusic, clickSound, victoryMusic, defeatMusic;
Sounds captainTheme, pirateTheme, spaceTheme;
Sounds captainLine, pirateLine, spaceLine;
Sounds hitSound, destroyedSound;

// Action points for the offline game mode
int offlineActionPoints = 2;
int onlineActionPoints = 2;

// Volume variables
int musicVolume = 5;
int buttonsVolume = 5;
int quotesVolume = 20;
int effectsVolume = 5 ;

// Variables for the game backgrounds
QPalette mainMenuBackground, secMenuBackground, gameBackground;
QString gameSkillInfoUI;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    // Iniciates random
    random_device rd;
    mt19937 gen(rd());

    // Defines the window
    ui->setupUi(this);
    ui->stackedWidget->setGeometry(0,0,960,720);
    ui->stackedWidget->setCurrentIndex(0);

    // Generates backgrounds
    backgroundLoad();

    // Generates the icons
    iconLoad();

    // Sets initial background
    this->setPalette(mainMenuBackground);

    // Creates a new game
    newGame();

    // Connects the function "textChange" to "validateName"
    connect(ui->create_insertname,&QPlainTextEdit::textChanged,this,&MainWindow::validateNameCreate);
    connect(ui->join_insertname,&QPlainTextEdit::textChanged,this,&MainWindow::validateNameJoin);

    // Plays game music
    loadSounds();
    menuMusic.playSound();
}

MainWindow::~MainWindow()
{
    stopSounds();
    delete ui;
}

void MainWindow::newGame()
{
    // Resets turn action points
    if(playstate == OFFLINE) offlineActionPoints = 2;
    else onlineActionPoints = 2;

    // Builds the player board and draws the layout
    player.myBoard.scene = new QGraphicsScene(this);
    ui->placement_board->setScene(player.myBoard.scene);
    ui->placement_board->setSceneRect(0,0,NUM_SQUARES*SQUARE,NUM_SQUARES*SQUARE);
    ui->placement_board->centerOn(QPointF(NUM_SQUARES*SQUARE/2,NUM_SQUARES*SQUARE/2));
    ui->placement_board->setMouseTracking(true);
    player.myBoard.DrawLayout();

    // Builds the enemy board and draws the layout
    player.enemyBoard.scene = new QGraphicsScene(this);
    ui->game_board->setScene(player.enemyBoard.scene);
    ui->game_board->setSceneRect(0,0,NUM_SQUARES*SQUARE,NUM_SQUARES*SQUARE);
    ui->game_board->centerOn(QPointF(NUM_SQUARES*SQUARE/2,NUM_SQUARES*SQUARE/2));
    ui->game_board->setMouseTracking(true);
    player.enemyBoard.DrawLayout();

    // Sets the preview
    player.enemyBoard.scene->addItem(&player.hero.preview);
    player.hero.preview.setBrush(*player.enemyBoard.greenPaint);
    player.hero.preview.setPen(*player.enemyBoard.blackLine);
    player.hero.preview.setZValue(1000);
    player.hero.preview.setOpacity(1);
    player.hero.preview.setFlags(QGraphicsItem::ItemIsMovable);
    player.hero.preview.setAcceptHoverEvents(true);

    // Bad name error messages
    ui->create_insertname_msg->hide();
    ui->join_insertname_msg->hide();
    ui->game_continue_button->setEnabled(false);



    // Resets end screen images
    ui->game_continue_button->setStyleSheet("background: transparent");
    ui->game_continue_button->setEnabled(false);
    ui->game_victory->setGeometry(0,0,0,0);
    ui->game_victory->setEnabled(false);
    ui->game_victory->setStyleSheet("background: transparent");
    ui->enemy_defeat->setStyleSheet("background: transparent");
    ui->enemy_defeat->setEnabled(false);
    ui->enemy_defeat->setGeometry(0,0,0,0);
    ui->enemy_continue_button->setEnabled(false);
    ui->enemy_continue_button->setStyleSheet("background: transparent");

    // Unsets icons
    setHeroIconLow(ui->create_captain_name,ui->create_captain_icon);
    setHeroIconLow(ui->create_pirate_name,ui->create_pirate_icon);
    setHeroIconLow(ui->create_space_name,ui->create_space_icon);

    // Resets in game skill info GUI
    resetGameSkillInfoGUI();

    // Hides warning messages
    ui->game_skill1_warning->hide();
    ui->game_skill2_warning->hide();
    ui->game_ultimate_warning->hide();
    ui->game_normal_warning->hide();


    ui->enemy_title->setText("Enemy turn");
    ui->enemy_title->setAlignment(Qt::AlignCenter);
    ui->enemy_myturn_button->setEnabled(false);

}

void MainWindow::deletePlayerGame()
{
    // Removes the elements from the scenes
    //
    // Removes all item from myBoard scene
    QList<QGraphicsItem*> allItems = player.myBoard.scene->items();
    for(int i = (player.myBoard.scene->items().size() -1 ); i <= 0; i--)
    {
        QGraphicsItem *item = allItems[i];
        player.myBoard.scene->removeItem(item);
    }

    // Removes all item from enemyBoard scene
    allItems.clear();
    allItems = player.enemyBoard.scene->items();
    for(int i = (player.enemyBoard.scene->items().size() - 1); i <= 0; i--)
    {
        QGraphicsItem *item = allItems[i];
        player.enemyBoard.scene->removeItem(item);
    }

    // Cleans the GUI
    ui->placement_board->resetTransform();
    ui->enemy_board->resetTransform();
    ui->game_board->resetTransform();
    ui->create_insertname->setPlainText("");


    // Resets "player"
    //
    // Resets character
    player.characterID = -1;
    player.hero.loadCharacter(player.characterID);

    // Resets objects
    for(int i = 0; i < NUM_SHIP; i++)
    {
        player.myShip[i].resetObject();
        player.enemyShip[i].resetObject();
    }


    // Resets the maps
    for(int i = 0; i < NUM_SQUARES; i++)
    {
        for(int j = 0; j < NUM_SQUARES; j++)
        {
            player.myMap[i][j] = false;
            player.myHits[i][j] = nothing;
            player.enemyHits[i][j] = nothing;
        }
    }

    // Removes player name
    player.playerName = "";

    // Disconnects
    if(playstate == OFFLINE)
    {
        disconnect(&player,&player::askHit,&bot,&cpu::respondHit);
        disconnect(&player,&player::askDestroyed,&bot,&cpu::respondDestroyed);
        disconnect(&player,&player::giveDestroyedPos,&bot,&cpu::drawCPUDestroyed);
        disconnect(&player,&player::checkVictory,&bot,&cpu::checkLost);
        // Connects bot to player
        disconnect(&bot,&cpu::askHit,&player,&player::respondHit);
        disconnect(&bot,&cpu::askDestroyed,&player,&player::respondDestroyed);
        disconnect(&bot,&cpu::giveDestroyedPos,&player,&player::drawPlayerDestroyed);
        disconnect(&bot,&cpu::checkVictory,&player,&player::checkLost);
    }

    // Enables objects mouve
    for(int i = 0; i < NUM_SHIP; i++) player.myShip[i].setAcceptedMouseButtons(Qt::LeftButton & Qt::RightButton);

}

void MainWindow::deleteCPUGame()
{
    // Resets character
    //
    bot.characterID = -1;
    bot.hero.loadCharacter(-1);

    // Resets objects
    for(int i = 0; i < NUM_SHIP; i++)
    {
        bot.myShip[i].resetObject();
        bot.enemyShip[i].resetObject();
    }

    // Resets the maps
    for(int i = 0; i < NUM_SQUARES; i++)
    {
        for(int j = 0; j < NUM_SQUARES; j++)
        {
            bot.myMap[i][j] = false;
            bot.myHits[i][j] = nothing;
            bot.enemyHits[i][j] = nothing;
        }
    }

}

void MainWindow::iconLoad()
{
    // Icon sizes
    QSize big = {195,192};
    QSize med = {120,117};

    // Captain icon
    QIcon icon(":/resources/squareicon_captain.png");
    // Create
    ui->create_captain_icon->setIcon(icon);
    ui->create_captain_icon->setIconSize(big);
    // Join
    ui->join_captain_icon->setIcon(icon);
    ui->join_captain_icon->setIconSize(big);
    // Info
    ui->info_captain_icon->setIcon(icon);
    ui->info_captain_icon->setIconSize(med);

    // Pirate icon
    icon.addFile(":/resources/squareicon_pirate.png");
    // Create
    ui->create_pirate_icon->setIcon(icon);
    ui->create_pirate_icon->setIconSize(big);
    // Join
    ui->join_pirate_icon->setIcon(icon);
    ui->join_pirate_icon->setIconSize(big);
    // Info
    ui->info_pirate_icon->setIcon(icon);
    ui->info_pirate_icon->setIconSize(med);

    // Space icon
    icon.addFile(":/resources/squareicon_spacec.png");
    // Create
    ui->create_space_icon->setIcon(icon);
    ui->create_space_icon->setIconSize(big);
    // Join
    ui->join_space_icon->setIcon(icon);
    ui->join_space_icon->setIconSize(big);
    // Info
    ui->info_space_icon->setIcon(icon);
    ui->info_space_icon->setIconSize(med);

}

bool MainWindow::validateNameCreate()
{
    // Gets what is written in the plain text
    QString inputName = ui->create_insertname->toPlainText();
    // Enter filter
    if(inputName.endsWith('\n'))
    {
        inputName.remove(inputName.length()-1,1);
        ui->create_insertname->setPlainText(inputName);
        QTextCursor cursor = ui->create_insertname->textCursor();
        cursor.setPosition(ui->create_insertname->toPlainText().length());
        ui->create_insertname->setTextCursor(cursor);
    }
    // Symbols and number of symbols permitted
    regex pattern("^[a-zA-Z0-9]{3,12}$");
    // Checks if the pattern is met
    bool valid = regex_match(inputName.toStdString(), pattern);
    // Hides or shows the error message
    if(valid) ui->create_insertname_msg->hide();
    else ui->create_insertname_msg->show();

    return valid;
}

bool MainWindow::validateNameJoin()
{
    // Gets what is written in the plain text
    QString inputName = ui->join_insertname->toPlainText();
    // Enter filter
    if(inputName.endsWith('\n'))
    {
        inputName.remove(inputName.length()-1,1);
        ui->join_insertname->setPlainText(inputName);
        QTextCursor cursor = ui->join_insertname->textCursor();
        cursor.setPosition(ui->join_insertname->toPlainText().length());
        ui->join_insertname->setTextCursor(cursor);
    }
    // Symbols and number of symbols permitted
    regex pattern("^[a-zA-Z0-9]{3,12}$");
    // Checks if the pattern is met
    bool valid = regex_match(inputName.toStdString(), pattern);
    // Hides or shows the error message
    if(valid) ui->join_insertname_msg->hide();
    else ui->join_insertname_msg->show();

    return valid;
}

bool MainWindow::validateCodeJoin()
{
    // Gets what is written in the plain text
    QString inputName = ui->join_insertcode->toPlainText();
    // Enter filter
    if(inputName.endsWith('\n'))
    {
        inputName.remove(inputName.length()-1,1);
        ui->join_insertcode->setPlainText(inputName);
        QTextCursor cursor = ui->join_insertcode->textCursor();
        cursor.setPosition(ui->join_insertcode->toPlainText().length());
        ui->join_insertcode->setTextCursor(cursor);
    }
    // Symbols and number of symbols permitted
    regex pattern("^[a-zA-Z0-9]{1,20}$");
    // Checks if the pattern is met
    bool valid = regex_match(inputName.toStdString(), pattern);

    return valid;
}

void MainWindow::backgroundLoad()
{
    // Main menu
    QPixmap pix(":/resources/menu_background1.png");
    pix = pix.scaled(this->size(),Qt::IgnoreAspectRatio);
    mainMenuBackground.setBrush(QPalette::Window, pix);

    // Sec menu
    pix.load(":/resources/menu_background2.png");
    pix = pix.scaled(this->size(),Qt::IgnoreAspectRatio);
    secMenuBackground.setBrush(QPalette::Window, pix);

    // Game
    pix.load(":/resources/placement_background.png");
    pix = pix.scaled(this->size(),Qt::IgnoreAspectRatio);
    gameBackground.setBrush(QPalette::Window, pix);

}

void MainWindow::setSkill(skill& skill)
{
    if(player.hero.primary.placed) unsetSkill(player.hero.primary);
    else if(player.hero.secondary.placed) unsetSkill(player.hero.secondary);
    else if(player.hero.ultimate.placed) unsetSkill(player.hero.ultimate);

    skill.placed = true;
    player.hero.preview.setRect(0,0,skill.getAoeSize().x*SQUARE,skill.getAoeSize().y*SQUARE);
    player.hero.preview.setPos(0,0);
    player.setActionPoints(player.getActionPoints()-skill.getCost());
    ui->game_board_button->setText("Use " + skill.name);
    ui->game_board_button->setEnabled(true);
    ui->game_actionp->setValue(player.getActionPoints());

}

void MainWindow::unsetSkill(skill& skill)
{
    // Updates players action points
    skill.placed = false;
    skill.cooldown = 0;
    player.hero.preview.setRect(0,0,0,0);
    player.setActionPoints(player.getActionPoints()+skill.getCost());
    ui->game_actionp->setValue(player.getActionPoints());
    ui->game_board_button->setText("Select a skill");
    ui->game_board_button->setEnabled(false);
}

void MainWindow::resetPlayerTurn()
{
    if(player.hero.normal.placed) unsetSkill(player.hero.normal);
    else if(player.hero.primary.placed) unsetSkill(player.hero.primary);
    else if(player.hero.secondary.placed) unsetSkill(player.hero.secondary);
    else if(player.hero.ultimate.placed) unsetSkill(player.hero.ultimate);

    player.hero.normal.used = false;
    player.hero.primary.used = false;
    player.hero.secondary.used = false;
    player.hero.ultimate.used = false;

    if(playstate == OFFLINE) player.setActionPoints(offlineActionPoints);
    else
    {
        player.setActionPoints(onlineActionPoints);

        // Updates action points bar
        ui->game_actionp->setMaximum(player.getActionPoints());
        ui->game_actionp->setValue(player.getActionPoints());

        // Updates ui
        ui->game_board_button->setText("Select a skill");
        ui->game_board_button->setEnabled(false);
    }
    player.setInterest(0);

    if(player.hero.ultimate.cooldown > 0) player.hero.ultimate.cooldown--;
}

void MainWindow::resetCPUTurn()
{
    bot.hero.primary.used = false;
    bot.hero.secondary.used = false;
    bot.hero.ultimate.used = false;
    bot.setActionPoints(offlineActionPoints);
    bot.setInterest(0);
}

bool MainWindow::setBoardBackground(int ID)
{
    if(ID == CAPTAIN_ID)
    {
        ui->placement_board->setStyleSheet("background-image: url(:/resources/captain_background.png); border: 9px solid #888888");
        ui->enemy_board->setStyleSheet("background-image: url(:/resources/captain_background.png); border: 9px solid #888888");
        ui->game_skill_ui->setStyleSheet("border-image: url(:/resources/captain_skillui.png); border: 0px");
        return true;
    }
    else if(ID == PIRATE_ID)
    {
        ui->placement_board->setStyleSheet("background-image: url(:/resources/pirate_background.png); border: 9px solid #888888");
        ui->enemy_board->setStyleSheet("background-image: url(:/resources/pirate_background.png); border: 9px solid #888888");
        ui->game_skill_ui->setStyleSheet("border-image: url(:/resources/pirate_skillui.png); border: 0px");
        return true;
    }
    else if(ID == SPACE_ID)
    {
        ui->placement_board->setStyleSheet("background-image: url(:/resources/space_background.png); border: 9px solid #888888");
        ui->enemy_board->setStyleSheet("background-image: url(:/resources/space_background.png); border: 9px solid #888888");
        ui->game_skill_ui->setStyleSheet("border-image: url(:/resources/space_skillui.png); border: 0px");
        return true;
    }
    else
    {
        ui->create_selectchar_label->setStyleSheet("font-weight: bold; color: red;");
        return false;
    }
}

void MainWindow::loadBot()
{
    // Creates the BOT
    bot.makeBoard();
    bot.makeEnemyShips();

    if(bot.characterID == CAPTAIN_ID) ui->game_board->setStyleSheet("background-image: url(:/resources/captain_background.png); border: 9px solid #888888");
    else if(bot.characterID == PIRATE_ID) ui->game_board->setStyleSheet("background-image: url(:/resources/pirate_background.png); border: 9px solid #888888");
    else if(bot.characterID == SPACE_ID) ui->game_board->setStyleSheet("background-image: url(:/resources/space_background.png); border: 9px solid #888888");

    bot.setInterest(0);
    bot.setActionPoints(2);

    ui->enemy_myturn_button->setEnabled(true);
}

void MainWindow::disableObjectsMove()
{
    for(int i = 0; i < NUM_SHIP; i++)
    {
        player.myShip[i].setAcceptedMouseButtons(Qt::XButton1);
        player.enemyShip[i].setAcceptedMouseButtons(Qt::XButton1);
        bot.myShip[i].setAcceptedMouseButtons(Qt::XButton1);
    }

}

void MainWindow::setHeroIconHigh(QLabel *label, QPushButton *button)
{

    label->setStyleSheet("color: green");
    button->setStyleSheet("border: 5px solid green");

}

void MainWindow::setHeroIconLow(QLabel *label, QPushButton *button)
{
    label->setStyleSheet("color: white");
    button->setStyleSheet("border: 5px solid grey");
}

void MainWindow::setVictory()
{
    if(player.characterID == CAPTAIN_ID) captainTheme.stop();
    else if(player.characterID == PIRATE_ID) pirateTheme.stop();
    else if(player.characterID == SPACE_ID) spaceTheme.stop();

    victoryMusic.playSound();

    // Victory image
    //
    // Set the position and size
    ui->game_victory->setGeometry(0,0,960,720);

    // Apply the stylesheet
    ui->game_victory->setStyleSheet("image: url(:/resources/victory.png) 2 2 2 2 stretch stretch;"
                          "background-color: rgba(0, 0, 0, 150);");

    // Add the image
    ui->game_victory->setEnabled(true);

    // Continue button
    //
    // Apply the stylesheet
    ui->game_continue_button->setStyleSheet("background:transparent;"
                          "image: url(:/resources/continue.png);");

    // Add the button
    ui->game_continue_button->setEnabled(true);
    ui->game_continue_button->raise();

}

void MainWindow::setDefeat()
{
    if(player.characterID == CAPTAIN_ID) captainTheme.stop();
    else if(player.characterID == PIRATE_ID) pirateTheme.stop();
    else if(player.characterID == SPACE_ID) spaceTheme.stop();

    defeatMusic.playSound();

    // Defeat image
    //
    // Set the position and size
    ui->enemy_defeat->setGeometry(0,0,960,720);

    // Apply the stylesheet
    ui->enemy_defeat->setStyleSheet("image: url(:/resources/defeat.png) 2 2 2 2 stretch stretch;"
                                    "background-color: rgba(0, 0, 0, 150);");

    // Add the image
    ui->enemy_defeat->setEnabled(true);

    // Continue button
    //
    // Apply the stylesheet
    ui->enemy_continue_button->setStyleSheet("background:transparent;"
                                            "image: url(:/resources/continue.png);");

    // Add the button
    ui->enemy_continue_button->setEnabled(true);
    ui->enemy_continue_button->raise();
}

void MainWindow::resetGameSkillInfoGUI()
{
    ui->game_skill_info->setGeometry(0,0,0,0);
    ui->game_skill_info_button->setEnabled(false);
    ui->game_skill_info_desc->clear();
    ui->game_skill_info_name->clear();
    ui->game_skill_info_button->setStyleSheet("background:transparent");
    ui->game_skill_info_button->setEnabled(false);
}

void MainWindow::hideGameWarning()
{
    ui->game_normal_warning->hide();
    ui->game_skill1_warning->hide();
    ui->game_skill2_warning->hide();
    ui->game_ultimate_warning->hide();
}

void MainWindow::showNotEnoughAP(QLabel* label)
{
    if(label->isVisible()) return;
    label->setText("Not enough action points.");
    label->show();
    QTimer::singleShot(2000,this,SLOT(hideGameWarning()));
}

void MainWindow::showOnCooldown(QLabel* label)
{
    if(label->isVisible()) return;
    label->setText("On cooldown.");
    label->show();
    QTimer::singleShot(2000,this,SLOT(hideGameWarning()));
}

void MainWindow::loadSounds()
{
    // Musics
    menuMusic.setSound(QUrl("qrc:/sounds/epicmenumusic.mp3"),musicVolume,true);
    victoryMusic.setSound(QUrl("qrc:/sounds/victory_music.mp3"),musicVolume,false);
    captainTheme.setSound(QUrl("qrc:/sounds/captaintheme.mp3"),musicVolume,true);
    pirateTheme.setSound(QUrl("qrc:/sounds/piratetheme.mp3"),musicVolume,true);
    spaceTheme.setSound(QUrl("qrc:/sounds/spacetheme.mp3"),musicVolume,true);
    victoryMusic.setSound(QUrl("qrc:/sounds/victory_music.mp3"),musicVolume,false);
    defeatMusic.setSound(QUrl("qrc:/sounds/defeat.mp3"),musicVolume,false);

    // Quotes
    captainLine.setSound(QUrl("qrc:/sounds/captain_line.mp3"),quotesVolume,false);
    pirateLine.setSound(QUrl("qrc:/sounds/pirate_line.mp3"),quotesVolume,false);
    spaceLine.setSound(QUrl("qrc:/sounds/space_line.mp3"),quotesVolume,false);

    // Click
    clickSound.setSound(QUrl("qrc:/sounds/clicksound.wav"),buttonsVolume,false);

    // Hit & Destroy
    hitSound.setSound(QUrl("qrc:/sounds/hit.mp3"),effectsVolume,false);
    destroyedSound.setSound(QUrl("qrc:/sounds/explosion.mp3"),effectsVolume,false);

    // Connects the slider with the variable
    connect(ui->settings_music_volume_slider,&QSlider::valueChanged,this,&MainWindow::changeMusicVolume);
    connect(ui->settings_effects_volume_slider,&QSlider::valueChanged,this,&MainWindow::changeEffectsVolume);
    connect(ui->settings_quotes_volume_slider,&QSlider::valueChanged,this,&MainWindow::changeQuotesVolume);

    // Changes slider value
    ui->settings_music_volume_slider->setValue(musicVolume);
    ui->settings_effects_volume_slider->setValue(effectsVolume);
    ui->settings_quotes_volume_slider->setValue(quotesVolume);
}

void MainWindow::changeMusicVolume()
{
    musicVolume = ui->settings_music_volume_slider->value();
    menuMusic.setVolume(static_cast<float>(musicVolume)/100);
    victoryMusic.setVolume(static_cast<float>(musicVolume)/100);
    captainTheme.setVolume(static_cast<float>(musicVolume)/100);
    pirateTheme.setVolume(static_cast<float>(musicVolume)/100);
    spaceTheme.setVolume(static_cast<float>(musicVolume)/100);
    victoryMusic.setVolume(static_cast<float>(musicVolume)/100);
    defeatMusic.setVolume(static_cast<float>(musicVolume)/100);

}

void MainWindow::changeEffectsVolume()
{
    effectsVolume = ui->settings_effects_volume_slider->value();
    destroyedSound.setVolume(static_cast<float>(effectsVolume)/100);
    hitSound.setVolume(static_cast<float>(effectsVolume)/100);
}

void MainWindow::changeQuotesVolume()
{
    quotesVolume = ui->settings_quotes_volume_slider->value();
    captainLine.setVolume(static_cast<float>(quotesVolume)/100);
    pirateLine.setVolume(static_cast<float>(quotesVolume)/100);
    spaceLine.setVolume(static_cast<float>(quotesVolume)/100);
}

void MainWindow::stopSounds()
{
    menuMusic.stop();
    clickSound.stop();
    captainTheme.stop();
    captainLine.stop();
    pirateTheme.stop();
    pirateLine.stop();
    spaceTheme.stop();
    spaceLine.stop();
    victoryMusic.stop();
    defeatMusic.stop();
}

void MainWindow::setInviteEnemyInfo()
{
    // Icon sizes
    QSize big = {195,192};

    QIcon icon;
    if(player.enemyCharacterID == CAPTAIN_ID)
    {
        icon.addFile(":/resources/squareicon_captain.png");
        ui->invite_c2_character->setText(CAPTAIN);
    }
    else if (player.enemyCharacterID  == PIRATE_ID)
    {
        icon.addFile(":/resources/squareicon_pirate.png");
        ui->invite_c2_character->setText(PIRATE);
    }
    else if (player.enemyCharacterID  == SPACE_ID)
    {
        icon.addFile(":/resources/squareicon_spacec.png");
        ui->invite_c2_character->setText(SPACE);
    }

    ui->invite_c2_icon->setIcon(icon);
    ui->invite_c2_icon->setIconSize(big);
    ui->invite_c2_name->setText(QString::fromStdString(player.enemyName));
    ui->invite_c2_name->setAlignment(Qt::AlignCenter);
    ui->invite_c2_character->setAlignment(Qt::AlignCenter);

    ui->invite_ready_button->setStyleSheet("QToolButton::hover{ color: green; background: transparent; font-size: 40px;}"
                                        "QToolButton {background: transparent; color: green; font-size: 30px;}");

    ui->invite_ready_button->setEnabled(true);
}

void MainWindow::sendMyInfo()
{
    QByteArray infoData;
    infoData.setNum(INFO_P,10);
    infoData.append(QString::number(player.characterID).toUtf8());
    infoData.append(QString::fromStdString(player.playerName).toUtf8());
    player.client->sendData(infoData);
}

void MainWindow::clientDataReceived(QString data)
{
    int packetCode = player.getCode(data.at(0));
    qDebug() << data << packetCode;

    switch (packetCode)
    {
        case INFO_P:
            player.enemyCharacterID = data.at(1).digitValue();
            data.removeFirst();
            data.removeFirst();
            player.enemyName = data.toStdString();
            qDebug() << "INFO_P" << player.enemyCharacterID;
            if(player.client->getID() == 1) setInviteEnemyInfo();
        break;

        case R_P:
            // Creates the ships and places them on the myBoard
            player.makePlayerShips();

            // Changes the background
            this->setPalette(gameBackground);

            // Plays the carachter music
            menuMusic.stop();
            if(player.characterID == CAPTAIN_ID) captainTheme.playSound();
            else if(player.characterID == PIRATE_ID) pirateTheme.playSound();
            else if(player.characterID == SPACE_ID) spaceTheme.playSound();

            // Changes window
            ui->stackedWidget->setCurrentWidget(ui->placement_background);
        break;

        case PLACE_R:
            if(player.client->getID() == 1)
            {
                sendMyInfo();
                ui->placement_ready_button->setEnabled(true);
                ui->placement_ready_button->setStyleSheet("QToolButton::hover{ color: green; background: transparent; font-size: 40px;}"
                                                          "QToolButton {background: transparent; color: green; font-size: 30px;}");
            }
            else if(player.client->getID() == 2)
            {
                placementReadyLeave();
            }
        break;

        case AHIT_P:
            player.waitPacket(data);
        break;

        case RHIT_P:
            emit player.sendSignal(data);
        break;

        case ADESTROY_P:
            player.waitPacket(data);
        break;

        case RDESTROY_P:
            emit player.sendSignal(data);
        break;

        case AVIC_P:
            if(player.waitPacket(data)) setDefeat();
        break;

        case RVIC_P:
            emit player.sendSignal(data);
        break;

        case TURN_P:
            ui->enemy_myturn_button->show();
            ui->enemy_myturn_button->setEnabled(true);
        break;

        case (int)ARADAR_P:
            player.waitPacket(data);
        break;

        case(int)RRADAR_P:
            emit player.sendSignal(data);
        break;

        default:
        break;
    }
}

void MainWindow::openServer()
{
    player.server = new Server;
    player.client = new Client;
    player.shareCode = player.server->startServer();
    connect(player.server, &Server::Client1Connected, this, &MainWindow::connectedC1);
    connect(player.server, &Server::Client2Connected, this, &MainWindow::connectedC2);
    connect(player.client, &Client::dataReady, this, &MainWindow::clientDataReceived);
    // connect(player.server, &Server::clientDisconnect, this, &MainWindow::clientDisconnected);
    ui->invite_sharecode->setText(player.shareCode);
    player.client->connectC1(player.server->whatPort());
}

void MainWindow::connectedC1()
{
    // Icon sizes
    QSize big = {195,192};

    // Character
    QIcon icon;
    if(player.characterID == CAPTAIN_ID)
    {
        icon.addFile(":/resources/squareicon_captain.png");
        ui->invite_c1_character->setText(CAPTAIN);
    }
    else if (player.characterID == PIRATE_ID)
    {
        icon.addFile(":/resources/squareicon_pirate.png");
        ui->invite_c1_character->setText(PIRATE);
    }
    else if (player.characterID == SPACE_ID)
    {
        icon.addFile(":/resources/squareicon_spacec.png");
        ui->invite_c1_character->setText(SPACE);
    }

    ui->invite_c1_icon->setIcon(icon);
    ui->invite_c1_icon->setIconSize(big);
    ui->invite_c1_name->setText(QString::fromStdString(player.playerName));
    ui->invite_c1_name->setAlignment(Qt::AlignCenter);
    ui->invite_c1_character->setAlignment(Qt::AlignCenter);
}

void MainWindow::connectedC2()
{
    qDebug() << "Client 2 connected in the MainWindow";
}

void MainWindow::placementReadyLeave()
{
    // Loads the character info
    player.hero.loadCharacter(player.characterID);
    if(player.characterID == CAPTAIN_ID) gameSkillInfoUI = "image: url(:/resources/captain_textBar.png)";
    else if(player.characterID == PIRATE_ID) gameSkillInfoUI = "image: url(:/resources/pirate_textBar.png)";
    else if(player.characterID == SPACE_ID) gameSkillInfoUI = "image: url(:/resources/space_textBar.png)";
    player.setActionPoints(2);
    player.setInterest(0);
    player.resetHits();

    // Gets the final ships positions
    for(int i = 0; i < NUM_SHIP; i++) player.myShip[i].getSquares();

    // Prepares the GUI
    ui->game_board_button->setText("Select a skill");
    ui->game_board_button->setEnabled(false);
    ui->game_actionp->setMaximum(player.getActionPoints());
    ui->game_actionp->setValue(player.getActionPoints());
    ui->game_skill1_label->setText(player.hero.primary.name +"  "+QString::number(player.hero.primary.getCost()));
    ui->game_skill2_label->setText(player.hero.secondary.name +"  "+QString::number(player.hero.secondary.getCost()));
    ui->game_ultimate_label->setText(player.hero.ultimate.name +"  "+QString::number(player.hero.ultimate.getCost()));
    ui->enemy_board->setScene(player.myBoard.scene);
    ui->enemy_board->setSceneRect(0,0,NUM_SQUARES*SQUARE,NUM_SQUARES*SQUARE);
    ui->enemy_board->centerOn(QPointF(NUM_SQUARES*SQUARE/2,NUM_SQUARES*SQUARE/2));

    // This label have the normal font size they wouldnt fit
    if(player.characterID == PIRATE_ID)
    {
        ui->game_skill1_label->setStyleSheet("color: white; font-size: 11px");
        ui->game_skill2_label->setStyleSheet("color: white; font-size: 12px");
    }

    // Disables the moving of objects
    disableObjectsMove();

    // Changes window
    if(playstate == JOIN) ui->stackedWidget->setCurrentWidget(ui->enemy_background);
    else ui->stackedWidget->setCurrentWidget(ui->game_background);

    // Loads enemy objects
    if(playstate != OFFLINE)
    {
        player.makeEnemyShips(player.enemyCharacterID);
        qDebug() << "My enemy" << player.enemyCharacterID;
        if(player.enemyCharacterID == CAPTAIN_ID) ui->game_board->setStyleSheet("background-image: url(:/resources/captain_background.png); border: 9px solid #888888");
        else if(player.enemyCharacterID == PIRATE_ID) ui->game_board->setStyleSheet("background-image: url(:/resources/pirate_background.png); border: 9px solid #888888");
        else if(player.enemyCharacterID == SPACE_ID) ui->game_board->setStyleSheet("background-image: url(:/resources/space_background.png); border: 9px solid #888888");

        // Connects
        connect(&player,&player::askHit,&player,&player::sendHit);
        connect(&player,&player::askDestroyed,&player,&player::sendDestroyed);
        connect(&player,&player::giveDestroyedPos,&player,&player::sendDestroyedPos);
        connect(&player,&player::askRadar,&player,&player::sendRadar);
        connect(&player,&player::checkVictory,&player,&player::waitVictoryResponse);
    }
}


// Main menu
void MainWindow::on_menu_play_button_clicked()
{
    clickSound.playSound();
    this->setPalette(secMenuBackground);
    ui->stackedWidget->setCurrentIndex(2);

}

void MainWindow::on_menu_options_button_clicked()
{
    clickSound.playSound();
    this->setPalette(secMenuBackground);
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_menu_quit_button_clicked()
{
    clickSound.playSound();
    this->close();
}

// Play menu
void MainWindow::on_playoptions_create_button_clicked()
{
    clickSound.playSound();
    ui->stackedWidget->setCurrentIndex(3);
    playstate = PLAYSTATE(CREATE);
}

void MainWindow::on_playoptions_join_button_clicked()
{
    clickSound.playSound();
    ui->stackedWidget->setCurrentIndex(5);
    playstate = PLAYSTATE(JOIN);
}

void MainWindow::on_playoptions_offline_button_clicked()
{
    clickSound.playSound();
    ui->stackedWidget->setCurrentWidget(ui->create_background);
    playstate = PLAYSTATE(OFFLINE);
}

void MainWindow::on_playoptions_back_button_clicked()
{
    clickSound.playSound();
    this->setPalette(mainMenuBackground);
    ui->stackedWidget->setCurrentIndex(0);
}

// Options menu
void MainWindow::on_options_settings_button_clicked()
{
    clickSound.playSound();
    ui->stackedWidget->setCurrentWidget(ui->settings_background);
    // Nothing happens yet
}

void MainWindow::on_options_instructions_button_clicked()
{
    clickSound.playSound();
    // Nothing happens yet
}

void MainWindow::on_options_credits_button_clicked()
{
    clickSound.playSound();
    // Nothing happens yet
}

void MainWindow::on_options_back_button_clicked()
{
    clickSound.playSound();
    this->setPalette(mainMenuBackground);
    ui->stackedWidget->setCurrentIndex(0);
}

// Settings
void MainWindow::on_settings_back_button_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->options_background);
}

// Create lobby menu
void MainWindow::on_create_captain_icon_clicked()
{

    captainLine.playSound();
    pirateLine.stop();
    spaceLine.stop();

    setHeroIconHigh(ui->create_captain_name,ui->create_captain_icon);
    setHeroIconLow(ui->create_pirate_name,ui->create_pirate_icon);
    setHeroIconLow(ui->create_space_name,ui->create_space_icon);
    player.characterID = CAPTAIN_ID;
}

void MainWindow::on_create_pirate_icon_clicked()
{
    captainLine.stop();
    pirateLine.playSound();
    spaceLine.stop();

    setHeroIconLow(ui->create_captain_name,ui->create_captain_icon);
    setHeroIconHigh(ui->create_pirate_name,ui->create_pirate_icon);
    setHeroIconLow(ui->create_space_name,ui->create_space_icon);
    player.characterID = PIRATE_ID;
}

void MainWindow::on_create_space_icon_clicked()
{
    captainLine.stop();
    pirateLine.stop();
    spaceLine.playSound();

    setHeroIconLow(ui->create_captain_name,ui->create_captain_icon);
    setHeroIconLow(ui->create_pirate_name,ui->create_pirate_icon);
    setHeroIconHigh(ui->create_space_name,ui->create_space_icon);
    player.characterID = SPACE_ID;
}

void MainWindow::on_create_start_button_clicked()
{
    clickSound.playSound();
    if(validateNameCreate())
    {
        if(!setBoardBackground(player.characterID)) return;
        player.playerName = ui->create_insertname->toPlainText().toStdString();
        if(playstate == CREATE)
        {
            // Creates server
            openServer();
            ui->placement_ready_button->setEnabled(false);
            ui->stackedWidget->setCurrentWidget(ui->invite_background);

        }
        else if(playstate == OFFLINE)
        {
            // Creates the ships and places them on the myBoard
            player.makePlayerShips();

            // Changes the background
            this->setPalette(gameBackground);
            ui->stackedWidget->setCurrentWidget(ui->placement_background);
        }

        menuMusic.stop();
        if(player.characterID == CAPTAIN_ID) captainTheme.playSound();
        else if(player.characterID == PIRATE_ID) pirateTheme.playSound();
        else if(player.characterID == SPACE_ID) spaceTheme.playSound();
    }

}

void MainWindow::on_create_back_button_clicked()
{
    clickSound.playSound();
    ui->stackedWidget->setCurrentIndex(2);
    // Resets char choice
    player.characterID = -1;
    setHeroIconLow(ui->create_captain_name,ui->create_captain_icon);
    setHeroIconLow(ui->create_pirate_name,ui->create_pirate_icon);
    setHeroIconLow(ui->create_space_name,ui->create_space_icon);

}

void MainWindow::on_create_info_button_clicked()
{
    clickSound.playSound();
    ui->stackedWidget->setCurrentWidget(ui->info_background);
}

// Join looby menu
void MainWindow::on_join_captain_icon_clicked()
{
    clickSound.playSound();
    setHeroIconHigh(ui->join_captain_name,ui->join_captain_icon);
    setHeroIconLow(ui->join_pirate_name,ui->join_pirate_icon);
    setHeroIconLow(ui->join_space_name,ui->join_space_icon);
    player.characterID = CAPTAIN_ID;
}

void MainWindow::on_join_pirate_icon_clicked()
{
    clickSound.playSound();
    setHeroIconLow(ui->join_captain_name,ui->join_captain_icon);
    setHeroIconHigh(ui->join_pirate_name,ui->join_pirate_icon);
    setHeroIconLow(ui->join_space_name,ui->join_space_icon);
    player.characterID = PIRATE_ID;
}

void MainWindow::on_join_space_icon_clicked()
{
    clickSound.playSound();
    setHeroIconLow(ui->join_captain_name,ui->join_captain_icon);
    setHeroIconLow(ui->join_pirate_name,ui->join_pirate_icon);
    setHeroIconHigh(ui->join_space_name,ui->join_space_icon);
    player.characterID = SPACE_ID;
}

void MainWindow::on_join_back_button_clicked()
{
    clickSound.playSound();
    ui->stackedWidget->setCurrentIndex(2);



    // Resets char choice
    player.characterID = -1;
    setHeroIconLow(ui->join_captain_name,ui->join_captain_icon);
    setHeroIconLow(ui->join_pirate_name,ui->join_pirate_icon);
    setHeroIconLow(ui->join_space_name,ui->join_space_icon);;
}

void MainWindow::on_join_start_button_clicked()
{
    clickSound.playSound();
    if(validateNameJoin() && validateCodeJoin() && player.characterID != -1)
    {
        setBoardBackground(player.characterID);
        player.playerName = ui->join_insertname->toPlainText().toStdString();
        player.client = new Client;
        player.client->connectC2(ui->join_insertcode->toPlainText());
        connect(player.client,&Client::dataReady,this,&MainWindow::clientDataReceived);
        sendMyInfo();
        ui->join_wait->setText("Wait for the host.");
        ui->join_wait->setAlignment(Qt::AlignCenter);
        ui->placement_ready_button->setText("Ready");

    }

}

// Waiting lobby
void MainWindow::on_invite_ready_button_clicked()
{

    clickSound.playSound();
    QByteArray data;
    data.append(QString::number(R_P).toUtf8());
    player.client->sendData(data);

    // Creates the ships and places them on the myBoard
    player.makePlayerShips();

    // Changes the background
    this->setPalette(gameBackground);
    menuMusic.stop();
    if(player.characterID == CAPTAIN_ID) captainTheme.playSound();
    else if(player.characterID == PIRATE_ID) pirateTheme.playSound();
    else if(player.characterID == SPACE_ID) spaceTheme.playSound();
    ui->stackedWidget->setCurrentWidget(ui->placement_background);
}

void MainWindow::on_invite_leave_button_clicked()
{
    clickSound.playSound();
   // Nothing happens yet
}

// Placement menu
void MainWindow::on_placement_back_button_clicked()
{
    clickSound.playSound();
    if(player.characterID == CAPTAIN_ID) captainTheme.stop();
    else if(player.characterID == PIRATE_ID) pirateTheme.stop();
    else if(player.characterID == SPACE_ID) spaceTheme.stop();
    for(int i = 0; i < NUM_SHIP; i++) player.myBoard.scene->removeItem(&player.myShip[i]);
    player.myBoard.scene->removeItem(&player.shadow);
    this->setPalette(mainMenuBackground);
    ui->stackedWidget->setCurrentIndex(0);

    // Resets char choice
    player.characterID = -1;
    setHeroIconLow(ui->create_captain_name,ui->create_captain_icon);
    setHeroIconLow(ui->create_pirate_name,ui->create_pirate_icon);
    setHeroIconLow(ui->create_space_name,ui->create_space_icon);
}

void MainWindow::on_placement_ready_button_clicked()
{
    clickSound.playSound();

    if(playstate == CREATE)
    {
        QByteArray data;
        data.append(QString::number(PLACE_R).toUtf8());
        player.client->sendData(data);
        placementReadyLeave();
    }
    else if(playstate == JOIN)
    {
        ui->enemy_myturn_button->hide();
        ui->enemy_myturn_button->setEnabled(false);

        QByteArray data;
        data.append(QString::number(PLACE_R).toUtf8());
        player.client->sendData(data);
    }
    else if(playstate == OFFLINE)
    {
        // Creates the bot
        loadBot();
        // Connects player to bot
        connect(&player,&player::askHit,&bot,&cpu::respondHit);
        connect(&player,&player::askDestroyed,&bot,&cpu::respondDestroyed);
        connect(&player,&player::giveDestroyedPos,&bot,&cpu::drawCPUDestroyed);
        connect(&player,&player::checkVictory,&bot,&cpu::checkLost);
        connect(&player,&player::askRadar,&bot,&cpu::respondRadar);
        // Connects bot to player
        connect(&bot,&cpu::askHit,&player,&player::respondHit);
        connect(&bot,&cpu::askDestroyed,&player,&player::respondDestroyed);
        connect(&bot,&cpu::giveDestroyedPos,&player,&player::drawPlayerDestroyed);
        connect(&bot,&cpu::checkVictory,&player,&player::checkLost);
        // Adds the bot ships
        player.makeEnemyShips(bot.characterID);
        // Initiates assets
        placementReadyLeave();

        // Changes window
        ui->stackedWidget->setCurrentWidget(ui->game_background);
    }
}

void MainWindow::on_placement_randomize_button_clicked()
{
    clickSound.playSound();
    player.randomizeBoard();
}

// Info menu
void MainWindow::on_info_captain_icon_clicked()
{
    clickSound.playSound();
    ui->info_pages->setCurrentWidget(ui->info_captain);
    ui->info_captain_icon->setStyleSheet("border: 5px solid green");
    ui->info_pirate_icon->setStyleSheet("border: 5px solid grey");
    ui->info_space_icon->setStyleSheet("border: 5px solid grey");
}

void MainWindow::on_info_pirate_icon_clicked()
{
    clickSound.playSound();
    ui->info_pages->setCurrentWidget(ui->info_pirate);
    ui->info_captain_icon->setStyleSheet("border: 5px solid grey");
    ui->info_pirate_icon->setStyleSheet("border: 5px solid green");
    ui->info_space_icon->setStyleSheet("border: 5px solid grey");
}

void MainWindow::on_info_space_icon_clicked()
{
    clickSound.playSound();
    ui->info_pages->setCurrentWidget(ui->info_space);
    ui->info_captain_icon->setStyleSheet("border: 5px solid grey");
    ui->info_pirate_icon->setStyleSheet("border: 5px solid grey");
    ui->info_space_icon->setStyleSheet("border: 5px solid green");
}

void MainWindow::on_info_back_button_clicked()
{
    clickSound.playSound();
    ui->stackedWidget->setCurrentWidget(ui->create_background);
}

// Game menu
void MainWindow::on_game_normal_button_clicked()
{
    clickSound.playSound();
    if(player.hero.normal.used)
    {
        // Signals the user
        showOnCooldown(ui->game_normal_warning);
        return;
    }

    // Unsets the other skills
    if(player.hero.normal.placed)
    {
        unsetSkill(player.hero.normal);
        return;
    }
    else if(player.hero.primary.placed) unsetSkill(player.hero.primary);
    else if(player.hero.secondary.placed) unsetSkill(player.hero.secondary);
    else if(player.hero.ultimate.placed) unsetSkill(player.hero.ultimate);

    // Checks if the skill can be used
    if(player.getActionPoints() < player.hero.normal.getCost())
    {
        showNotEnoughAP(ui->game_normal_warning);
        return;
    }

    // Sets this skill
    setSkill(player.hero.normal);
}

void MainWindow::on_game_skill1_button_clicked()
{
    clickSound.playSound();
    // Checks of the skill was used
    if(player.hero.primary.used)
    {
        // Signals the user
        showOnCooldown(ui->game_skill1_warning);
        return;
    }

    // Unsets the other skills
    if(player.hero.primary.placed)
    {
        unsetSkill(player.hero.primary);
        return;
    }
    else if(player.hero.normal.placed) unsetSkill(player.hero.normal);
    else if(player.hero.secondary.placed) unsetSkill(player.hero.secondary);
    else if(player.hero.ultimate.placed) unsetSkill(player.hero.ultimate);

    // Checks if the skill can be used
    if(player.getActionPoints() < player.hero.primary.getCost())
    {
        showNotEnoughAP(ui->game_skill1_warning);
        return;
    }

    // Sets this skill
    setSkill(player.hero.primary);

}

void MainWindow::on_game_skill2_button_clicked()
{
    clickSound.playSound();
    // Checks of the skill was used
    if(player.hero.secondary.used)
    {
        // Signals the user
        showOnCooldown(ui->game_skill2_warning);
        return;
    }

    // Unsets the other skills
    if(player.hero.secondary.placed)
    {
        unsetSkill(player.hero.secondary);
        return;
    }
    else if(player.hero.normal.placed) unsetSkill(player.hero.normal);
    else if(player.hero.primary.placed) unsetSkill(player.hero.primary);
    else if(player.hero.ultimate.placed) unsetSkill(player.hero.ultimate);

    // Checks if the skill can be used
    if(player.getActionPoints() < player.hero.secondary.getCost())
    {
        showNotEnoughAP(ui->game_skill2_warning);
        return;
    }

    // Sets this skill
    setSkill(player.hero.secondary);

}

void MainWindow::on_game_ultimate_button_clicked()
{
    clickSound.playSound();
    // Checks of the skill was used
    if(player.hero.ultimate.used || player.hero.ultimate.cooldown != 0)
    {
        // Signals the user
        showOnCooldown(ui->game_ultimate_warning);
        return;
    }

    // Checks if any skill is placed
    if(player.hero.ultimate.placed)
    {
        unsetSkill(player.hero.ultimate);
        return;
    }
    else if(player.hero.normal.placed) unsetSkill(player.hero.normal);
    else if(player.hero.primary.placed) unsetSkill(player.hero.primary);
    else if(player.hero.secondary.placed) unsetSkill(player.hero.secondary);

    // Checks if the skill can be used
    if(player.getActionPoints() < player.hero.ultimate.getCost())
    {
        showNotEnoughAP(ui->game_ultimate_warning);
        return;
    }

    // Sets this skill
    setSkill(player.hero.ultimate);
    player.hero.ultimate.cooldown = 2;

}

void MainWindow::on_game_board_button_clicked()
{
    clickSound.playSound();

    // Uses whatever skill is placed
    if(player.hero.primary.placed){
        if(player.useSkill(player.hero.primary,destroyedSound))
            setVictory();
    }
    else if(player.hero.secondary.placed){
        if(player.useSkill(player.hero.secondary,destroyedSound))
            setVictory();
    }
    else if(player.hero.ultimate.placed){
        if(player.useSkill(player.hero.ultimate,destroyedSound))
            setVictory();
    }
    else if(player.hero.normal.placed){
        if(player.useSkill(player.hero.normal,destroyedSound))
            setVictory();
    }


    // Disables the use skill button
    ui->game_board_button->setEnabled(false);
    // Changes the button text
    if(player.getActionPoints() == 0) ui->game_board_button->setText("You have no action points");
    else ui->game_board_button->setText("Select a skill");

}

void MainWindow::on_game_turn_button_clicked()
{
    clickSound.playSound();

    // Changes window
    ui->stackedWidget->setCurrentWidget(ui->enemy_background);

    // Enemy turn player just watchs
    if(playstate == OFFLINE)
    {
        if(bot.makePlay()) setDefeat();
        ui->enemy_myturn_button->setEnabled(true);
        ui->enemy_myturn_button->show();
    }
    else
    {
        player.client->sendData(QString::number(TURN_P).toUtf8());
        resetPlayerTurn();
        ui->enemy_myturn_button->hide();
        ui->enemy_myturn_button->setEnabled(false);
    }
}

void MainWindow::on_game_continue_button_clicked()
{
    clickSound.playSound();
    victoryMusic.stop();
    menuMusic.playSound();

    // To remove the potencial preview
    if(player.hero.primary.placed) unsetSkill(player.hero.primary);
    else if(player.hero.secondary.placed) unsetSkill(player.hero.secondary);
    else if(player.hero.ultimate.placed) unsetSkill(player.hero.ultimate);

    // Restarts the game
    deletePlayerGame();
    if(playstate == OFFLINE) deleteCPUGame();
    newGame();

    // Sets background and back to the menu
    ui->stackedWidget->setCurrentWidget(ui->menu_background);
    this->setPalette(mainMenuBackground);

}

void MainWindow::on_game_normal_info_button_clicked()
{
    clickSound.playSound();
    ui->game_skill_info->setGeometry(0,0,960,720);
    ui->game_skill_info_button->setEnabled(true);
    ui->game_skill_info->setStyleSheet(gameSkillInfoUI);
    ui->game_skill_info_desc->setPlainText(player.hero.normal.description);
    ui->game_skill_info_name->setPlainText(player.hero.normal.name);
    ui->game_skill_info_button->setStyleSheet("background:transparent;"
                                              "image: url(:/resources/flat_cross.png) 2 2 2 2 stretch stretch");
    ui->game_skill_info_button->setEnabled(true);
}

void MainWindow::on_game_skill1_info_button_clicked()
{
    clickSound.playSound();
    ui->game_skill_info->setGeometry(0,0,960,720);
    ui->game_skill_info_button->setEnabled(true);
    ui->game_skill_info->setStyleSheet(gameSkillInfoUI);
    ui->game_skill_info_desc->setPlainText(player.hero.primary.description);
    ui->game_skill_info_name->setPlainText(player.hero.primary.name);
    ui->game_skill_info_button->setStyleSheet("background:transparent;"
                                              "image: url(:/resources/flat_cross.png) 2 2 2 2 stretch stretch");
    ui->game_skill_info_button->setEnabled(true);
}

void MainWindow::on_game_skill2_info_button_clicked()
{
    clickSound.playSound();
    ui->game_skill_info->setGeometry(0,0,960,720);
    ui->game_skill_info_button->setEnabled(true);
    ui->game_skill_info->setStyleSheet(gameSkillInfoUI);
    ui->game_skill_info_desc->setPlainText(player.hero.secondary.description);
    ui->game_skill_info_name->setPlainText(player.hero.secondary.name);
    ui->game_skill_info_button->setStyleSheet("background:transparent;"
                                              "image: url(:/resources/flat_cross.png) 2 2 2 2 stretch stretch");;
    ui->game_skill_info_button->setEnabled(true);
}

void MainWindow::on_game_ultimate_info_button_clicked()
{
    clickSound.playSound();
    ui->game_skill_info->setGeometry(0,0,960,720);
    ui->game_skill_info_button->setEnabled(true);
    ui->game_skill_info->setStyleSheet(gameSkillInfoUI);
    ui->game_skill_info_desc->setPlainText(player.hero.ultimate.description);
    ui->game_skill_info_name->setPlainText(player.hero.ultimate.name);
    ui->game_skill_info_button->setStyleSheet("background:transparent;"
                                              "image: url(:/resources/flat_cross.png) 2 2 2 2 stretch stretch");
    ui->game_skill_info_button->setEnabled(true);
}

void MainWindow::on_game_skill_info_button_clicked()
{
    clickSound.playSound();
    resetGameSkillInfoGUI();
}

void MainWindow::on_game_quit_clicked()
{
    clickSound.playSound();
    // To remove the potencial preview
    if(player.hero.normal.placed) unsetSkill(player.hero.normal);
    else if(player.hero.primary.placed) unsetSkill(player.hero.primary);
    else if(player.hero.secondary.placed) unsetSkill(player.hero.secondary);
    else if(player.hero.ultimate.placed) unsetSkill(player.hero.ultimate);

    // Restarts the game
    deletePlayerGame();
    if(playstate == OFFLINE) deleteCPUGame();
    newGame();

    // Sets background and back to the menu
    ui->stackedWidget->setCurrentWidget(ui->menu_background);
    this->setPalette(mainMenuBackground);
}

// Enemy menu
void MainWindow::on_enemy_myturn_button_clicked()
{
    clickSound.playSound();
    if(playstate == OFFLINE)
    {
        // Actions points are incresed by 1
        offlineActionPoints++;
        // Resets the CPU turn
        resetCPUTurn();
        // Resets player turn
        resetPlayerTurn();
        // Updates action points bar
        ui->game_actionp->setMaximum(player.getActionPoints());
        ui->game_actionp->setValue(player.getActionPoints());
        // Updates ui
        ui->game_board_button->setText("Select a skill");
        ui->game_board_button->setEnabled(false);
    }
    else
    {
        onlineActionPoints++;
        resetPlayerTurn();
    }

    ui->stackedWidget->setCurrentWidget(ui->game_background);
    ui->enemy_myturn_button->hide();
}

void MainWindow::on_enemy_continue_button_clicked()
{

    clickSound.playSound();
    menuMusic.playSound();
    defeatMusic.stop();


    // Restarts the game
    deletePlayerGame();
    if(playstate == OFFLINE) deleteCPUGame();
    newGame();

    // Sets background and back to the menu
    ui->stackedWidget->setCurrentWidget(ui->menu_background);
    this->setPalette(mainMenuBackground);
}





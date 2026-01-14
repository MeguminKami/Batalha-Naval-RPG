#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QPixmap>
#include <QGraphicsItem>
#include <QObject>
#include <QCursor>
#include <QTimer>
#include <QChar>
#include <regex>
#include "sounds.h"
#include "player.h"
#include "cpu.h"



namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Loads the menus backgrounds
    void backgroundLoad();

    // Loads the character icons
    void iconLoad();

    // Validates the name in the create menu
    bool validateNameCreate();

    // Validates the name in the create menu
    bool validateNameJoin();

    // Validates join code
    bool validateCodeJoin();

    // Sets the skill in the scene visually
    void setSkill(skill& skill);

    // Removes the skill from the scene visually
    void unsetSkill(skill& skill);

    // Resets the players turn variables (action points)
    void resetPlayerTurn();

    // Resets the bots turn variables (action points)
    void resetCPUTurn();

    // Sets the board background of the character you choose
    bool setBoardBackground(int ID);

    // Creates the bot
    void loadBot();

    // After placement all objects must not move
    void disableObjectsMove();

    // Highlight ON
    void setHeroIconHigh(QLabel *label, QPushButton *button);
    // Highlight OFF
    void setHeroIconLow(QLabel *label, QPushButton *button);

    // Sets the vicotory screen
    void setVictory();

    // Sets the defeat screen
    void setDefeat();

    // Creates the variables used in the game
    void newGame();

    // Deletes the variables used in the game by the player
    void deletePlayerGame();

    // Deletes the variables used in the game by the CPU
    void deleteCPUGame();

    // Read the name
    void resetGameSkillInfoGUI();

    // Shows the not enough ap warning message
    void showNotEnoughAP(QLabel* label);

    // Shows on cooldown warning message
    void showOnCooldown(QLabel* label);

    // Load sounds
    void loadSounds();

    // Stop all sounds
    void stopSounds();

    // Changes the music volume
    void changeMusicVolume();

    // Changes the effects volume
    void changeEffectsVolume();

    // Changes the quotes volume
    void changeQuotesVolume();

    // Opens the server and connects player 1
    void openServer();

    // On client 1 connecting
    void connectedC1();

    // On client 2 connecting
    void connectedC2();

    // Client data received
    void clientDataReceived(QString data);

    // Sends player name and charID
    void sendMyInfo();

    void setInviteEnemyInfo();

    void placementReadyLeave();



public slots:
   // Removes any warning message
   void hideGameWarning();

private:
    Ui::MainWindow *ui;

private slots:


    // Main menu
    void on_menu_play_button_clicked();
    void on_menu_options_button_clicked();
    void on_menu_quit_button_clicked();

    // Play options
    void on_playoptions_create_button_clicked();
    void on_playoptions_join_button_clicked();
    void on_playoptions_back_button_clicked();
    void on_playoptions_offline_button_clicked();

    // Options
    void on_options_settings_button_clicked();
    void on_options_instructions_button_clicked();
    void on_options_credits_button_clicked();
    void on_options_back_button_clicked();

    // Create Game
    void on_create_captain_icon_clicked();
    void on_create_pirate_icon_clicked();
    void on_create_space_icon_clicked();
    void on_create_start_button_clicked();
    void on_create_back_button_clicked();
    void on_create_info_button_clicked();

    // Join Game
    void on_join_captain_icon_clicked();
    void on_join_pirate_icon_clicked();
    void on_join_space_icon_clicked();
    void on_join_back_button_clicked();
    void on_join_start_button_clicked();

    // Waiting lobby
    void on_invite_leave_button_clicked();
    void on_invite_ready_button_clicked();

    // Placement
    void on_placement_back_button_clicked();
    void on_placement_ready_button_clicked();
    void on_placement_randomize_button_clicked();

    // Info
    void on_info_captain_icon_clicked();
    void on_info_pirate_icon_clicked();
    void on_info_space_icon_clicked();
    void on_info_back_button_clicked();

    // Player game turn
    void on_game_skill1_button_clicked();
    void on_game_skill2_button_clicked();
    void on_game_ultimate_button_clicked();
    void on_game_board_button_clicked();
    void on_game_turn_button_clicked();
    void on_game_continue_button_clicked();

    // Enemy game turn
    void on_enemy_myturn_button_clicked();
    void on_enemy_continue_button_clicked();
    void on_game_skill1_info_button_clicked();
    void on_game_skill2_info_button_clicked();
    void on_game_ultimate_info_button_clicked();
    void on_game_skill_info_button_clicked();
    void on_game_quit_clicked();
    void on_game_normal_button_clicked();
    void on_game_normal_info_button_clicked();
    void on_settings_back_button_clicked();
};
#endif // MAINWINDOW_H

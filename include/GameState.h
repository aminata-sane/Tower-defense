#ifndef GAMESTATE_H
#define GAMESTATE_H

enum class GameState {
    LOADING,    // État initial de chargement des ressources
    MENU,       // État du menu principal
    PLAYING,    // Le jeu est en cours
    PAUSED,     // Le jeu est en pause
    VICTORY,    // Le joueur a gagné
    DEFEAT,     // Le joueur a perdu
    EXITING     // Le jeu est en train de se fermer
};

#endif // GAMESTATE_H
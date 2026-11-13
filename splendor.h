/**
 * Nous nous proposons de modéliser le jeu Splendor édité chez Space Cowboys.
 *
 * [ Il n'est clairement pas supposé que vous connaissiez déjà les règles, les
 * questions sont là pour les expliquer. ]
 *
 * Le jeu utilisé des cartes (card_t) qui ont un score et fournissent une unité
 * d'un type de ressources (color_t).  De plus elles ont un cout en ressources
 * (par ex, White: 1, Red 2). Ce cout est matérialisé par un tableau d'entier
 * indicé par la couleur.
 **/

enum color_t { WHITE, BLUE, GREEN, RED, MAROON, LAST_COLOR };

struct card_t {
    int points;
    enum color_t provides;
    int cost[LAST_COLOR];
};

/**
 * Durant la partie, une joueur va accumuler des cartes, la structure suivante
 * (state_t) synthétise (résume) cette information pour un joueur.  Le tableau
 * ressources est l'ensemble de ressources fournies (quantité) indicé par la
 * couleur.
 *
 * (Plus de détail à la question `compute_state`)
 */
struct state_t {
    int score;
    int resources[LAST_COLOR];
};

/**
 * Les fonctions `str_search` et `parse_resources` sont des fonctions de
 * manipulations de chaîne, elles n'utilisent aucune structure.
 *
 * Barème indicatif :
 * - 1 pt/fonction : print_resources, print_card, score, have_enough,
 *                   noble_visit, missing_resources, buy, buyable_cards,
 *                   str_search
 * - 2 pt/fonction : compute_state, can_pick
 * - 3 pt/fonction : parse_resources, most_frequent_color
 */

/**
 * Affiche le nombre de ressources disponibles, s'il n'est pas nul, pour chaque
 * couleur sous la forme :
 *
 * "Red: 2, Green: 1"
 *
 * RQ: Chaque couple couleur/nombre est séparé par une virgule. Il n'y a pas
 *     de virgule inutile.
 * RQ: L'ordre est donné par celui de l'énumération color_t
 * RQ: Il n'y a pas de saut de ligne final
 */
void print_resources(int resources[])
    ;

/**
 * Affiche la carte `card` sous la forme :
 *
 * Needed: Red: 2, Green: 1
 * Provides: White
 * Points: 3
 *
 * RQ: Chaque ligne est terminée par un saut de ligne
 */
void print_card(struct card_t card)
    ;

/**
 * Retourne la somme des points contenu dans l'ensemble de cartes `deck`
 */
int score(int deck_size, struct card_t deck[])
    ;

/**
 * Cette fonction synthétise l'information contenue par l'ensemble de cartes
 * `deck` sous la forme d'un état (state_t).
 * Les ressources de l'état correspondent à l'accumulation des ressources
 * produites par les cartes du deck.
 *
 * RQ: Le coût n'intervient pas dans l'état.
 */
struct state_t compute_state(int deck_size, struct card_t deck[])
    ;

/**
 * Ce prédicat retourne 1 si les ressources disponibles sont
 * suffisantes pour couvrir le coût d'une carte, 0 sinon.
 *
 * RQ: Cette fonction est un cas particulier (simplifié) de la fonction
 *    `missing_resources`.
 */
int have_enough(struct state_t state, struct card_t card)
    ;

/**
 * Retourne la position de la première carte qui vérifie `have_enough`;
 * et -1 s'il n'existe pas une telle carte
 */
int noble_visit(struct state_t state, int nobles_count, struct card_t nobles[])
    ;

/**
 * Retourne 0 si la carte `card` peut être achetée. Pour acheter une carte
 * il faut disposer des ressources (`state`) qui peuvent être complétées
 * par des jetons dans le `wallet`. Si l'achat est impossible la fonction
 * retourne le nombre de jetons manquant.
 *
 * Ex: carte: 2 bleu, 2 vert
 *     état : 1 bleu, 1 vert
 * si wallet 1 rouge, 1 bleu, 1 vert    => achat possible   => retourne 0
 * si wallet 1 blanc, 1 rouge, 1 marron => achat impossible => retourne 2
 */
int missing_resources(struct state_t state, const int wallet[],
        struct card_t card)
    ;

/**
 * Achète la carte `card`, i.e., dépense le minimum de ressources de `wallet`.
 * La modification est faite par effet de bord sur `wallet`.
 *
 * RQ: Il est supposé que le joueur dispose d'assez de ressources pour acheter
 *     la carte i.e., cette fonction ne peut pas échouer.
 *
 * Ex: carte : {[BLUE]=2, [GREEN]=2}
 *     état  : {[BLUE]=1, [GREEN]=1}
 *     wallet: {[BLUE]=1, [GREEN]=1, [RED]=1}
 * Après l'appel =>
 *     wallet: {[RED]=1}
 */
void buy(struct state_t state, int wallet[], struct card_t card)
    ;

/**
 * Sélectionne l'ensemble des cartes achetables d'un deck en fonction
 * d'un état (state) et d'un porte monnaie (wallet).
 * Les cartes sélectionnées sont stockées dans le tableau `buyable` et la
 * fonction retourne le nombre d'éléments sélectionnés.
 */
int buyable_cards(struct state_t state, int wallet[],
        int deck_size, struct card_t deck[], struct card_t buyable[])
    ;

/**
 * Retourne 0 si le joueur fait une infraction aux règles de piochage,
 *    1 sinon.
 *
 * Règles de piochages :
 *  - Au plus trois jetons
 *  - 3 jetons de couleurs différentes, si les jetons sont disponibles.
 *  - seulement 2 jetons, si deux jetons de la même couleur demandé et si la
 *    pioche contient au moins 4 jetons
 *    de cette couleur.
 */
int can_pick(const int wanted[], const int stacks[])
    ;

/**
 * Retourne un pointeur sur la première occurrence du caractère `c` dans la
 * chaine `str`, ou un pointeur sur la fin de la chaîne.
 *
 * RQ: C'est l'équivalent de la fonction `strchrnul` (GNU), une variante de la
 * fonction `strchr` (POSIX). Vous pouvez vous inspirer de la man page
 * correspondante. Vous ne pouvez cependant pas les appeler.
 */
const char* str_search(const char* str, char c)
    ;

/**
 * Remplit `wallet` avec les ressources indiquées dans la chaîne de
 * caractères `str`. `str` contient au maximum 5 nombres séparés par
 * exactement un espace (sauf le dernier).
 *
 * Ex: "5 321345 122 6 0" -> {5, 321345, 122, 6, 0}
 *
 * RQ: La chaîne d'entrée ne doit pas être modifiée.
 * RQ: Vous pouvez utiliser `atoi` ou `strtol`
 * RQ: Même si vous ne gérez pas tout les cas, vous aurez une partie
 *     (significative) des points.
 */
void parse_resources(const char *str, int wallet[])
    ;

/**
 * Étant donné un tableau de cartes `deck`, déterminer la ressource
 * apparaissant le plus souvent. Une ressource apparaît sur une carte
 * si et seulement si son coût est strictement positif. Si il y a
 * plusieurs ressources apparaissant aussi souvent, renvoyer la
 * première dans l'ordre des couleurs (`enum color_t`).
 *
 * Ex: {{[RED]=1, [BLUE]=66}, {[RED]=1, [GREEN]=2}}   -> RED
 * Ex: {{[RED]=1, [BLUE]=66}, {[WHITE]=1, [GREEN]=2}} -> WHITE
 */
enum color_t most_frequent_color(int deck_size, struct card_t deck[])
    ;

enum color { WHITE, BLUE, GREEN, RED, MAROON, LAST_COLOR };

struct card {
    int points;
    enum color provides;
    int cost[LAST_COLOR];
};

struct noble {
    int points;
    int cost[LAST_COLOR];
    const char *name;
};

// TODO:
//  - add max lines for paths
//  - fix game over screen
//  - line collision


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <raylib.h>


#define X 1000
#define Y 800

#define N 30 // number of points
#define R 7  // radius of points

#define BORDER 100 // distance from X and Y
#define RECT_SIZE 20 
#define LINE_THICKNESS 2

int yellow_count = 1;
int blue_count = 1;

float rand_float();

void init_points(Vector2 points[]);
void draw_points(Vector2 points[]);

int choose_turn(int turn);
bool clicked(Vector2 current_pos, Vector2 mouse_pos);

void create_path(Vector2 points[], Vector2 yellow_path[], Vector2 blue_path[], Vector2 mouse_pos, int *turn);
void draw_lines(Vector2 yellow_path[], Vector2 blue_path[]);

bool is_intersecting(Vector2 start1, Vector2 end1, Vector2 start2, Vector2 end2);
void line_collision(Vector2 yellow_path[], Vector2 blue_path[], bool *quit);

bool game_over(Vector2 mouse_pos, Rectangle yellow, Rectangle blue);


int main() {
    srand(time(NULL));

    const Rectangle yellow = {BORDER, Y - BORDER - RECT_SIZE, RECT_SIZE, RECT_SIZE};
    const Rectangle blue = {X - BORDER - RECT_SIZE, BORDER, RECT_SIZE, RECT_SIZE};

    Vector2 points[N];

    Vector2 yellow_path[N];
    yellow_path[0].x = yellow.x + RECT_SIZE / 2;
    yellow_path[0].y = yellow.y + RECT_SIZE / 2;

    Vector2 blue_path[N];
    blue_path[0].x = blue.x + RECT_SIZE / 2;
    blue_path[0].y = blue.y + RECT_SIZE / 2;

    int turn = 0;
    bool quit = false;

    init_points(points);

    InitWindow(X,Y, "points");
    SetTargetFPS(60);       


    while (!WindowShouldClose() && !quit) {
        Vector2 mouse_pos = GetMousePosition();
        quit = game_over(mouse_pos, yellow, blue);

        create_path(points, yellow_path, blue_path, mouse_pos, &turn);
        line_collision(yellow_path, blue_path, &quit);

        BeginDrawing();
        ClearBackground(GRAY);

        choose_turn(turn);
        draw_points(points);
        draw_lines(yellow_path, blue_path);

        DrawRectangleRec(yellow, YELLOW);
        DrawRectangleRec(blue, BLUE);

        DrawText(TextFormat("N: %d", N), X - 90, 10, 30, RED);
        
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

float rand_float() {
    return (float)rand() / (float)RAND_MAX;
}

void init_points(Vector2 points[]) {
    for (int i=0; i<N; i++) {
        points[i].x = BORDER + rand_float() * (X - BORDER * 2);
        points[i].y = BORDER + rand_float() * (Y - BORDER * 2);
    }
}

void draw_points(Vector2 points[]) {
    for (int i=0; i<N; i++) {
        DrawCircle(points[i].x, points[i].y, R, BLACK);
        DrawRectangleLines(BORDER, BORDER, X - 2 * BORDER, Y - 2 * BORDER, RED);
        /* DrawText(TextFormat("%d", i), x+4, y+4, 30, RED); */
    }
}

bool clicked(Vector2 current_pos, Vector2 mouse_pos) {
    if (CheckCollisionPointCircle(mouse_pos, current_pos, R) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        return true;
    return false;
}

bool game_over(Vector2 mouse_pos, Rectangle yellow, Rectangle blue) {
    if (CheckCollisionPointRec(mouse_pos, yellow) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        DrawText("BLUE WON", 200,200, 100, BLUE);
        return true;
    }
    else if (CheckCollisionPointRec(mouse_pos, blue) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        DrawText("YELLOW WON", 200,200, 100, YELLOW);
        return true;
    }
    return false;
}

int choose_turn(int turn) {
    if (turn % 2 == 0) {
        DrawText("YELLOW'S TURN", 10, 10, 30, BLACK);
        return 0;
    } else {
        DrawText("BLUE'S TURN", 10, 10, 30, BLACK);
        return 1;
    }
}

void create_path(Vector2 points[], Vector2 yellow_path[], Vector2 blue_path[], Vector2 mouse_pos, int *turn) {
    for (int i=0; i<N; i++) {

        if (clicked(points[i], mouse_pos)) {
            if (choose_turn(*turn) == 0) {
                yellow_path[yellow_count] = points[i];
                yellow_count++;
            } 

            else {
                blue_path[blue_count] = points[i];
                blue_count++;
            }

            (*turn)++;
        }
    }
}

void draw_lines(Vector2 yellow_path[], Vector2 blue_path[]) {
    for (int i=1; i<yellow_count; i++) {
        DrawLineEx(yellow_path[i], yellow_path[i-1], LINE_THICKNESS, YELLOW);
    }
    for (int i=1; i<blue_count; i++) {
        DrawLineEx(blue_path[i], blue_path[i-1], LINE_THICKNESS, BLUE);
    }
}

bool is_intersecting(Vector2 start1, Vector2 end1, Vector2 start2, Vector2 end2) {
    float denominator = ((end1.x - start1.x) * (end2.y - start2.y)) - ((end1.y - start1.y) * (end2.x - start2.x));
    float numerator1 = ((start1.y - start2.y) * (end2.x - start2.x)) - ((start1.x - start2.x) * (end2.y - start2.y));
    float numerator2 = ((start1.y - start2.y) * (end1.x - start1.x)) - ((start1.x - start2.x) * (end1.y - start1.y));

    if (denominator == 0)
        return numerator1 == 0 && numerator2 == 0;
    
    float r = numerator1 / denominator;
    float s = numerator2 / denominator;

    return (r > 0 && r < 1) && (s > 0 && s < 1);
}

void line_collision(Vector2 yellow_path[], Vector2 blue_path[], bool *quit) {
    printf(".....................\n");
    for (int i=0; i<yellow_count; i += 2) {
        for (int j=0; j<blue_count; j += 2) {
            printf("y %d, b %d\n", yellow_count, blue_count);
            *quit = is_intersecting(yellow_path[i], yellow_path[i+1], blue_path[j], blue_path[j+1]);
        }
    }
}

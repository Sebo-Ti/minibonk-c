#include "raylib.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 1200
#define HEIGHT 800
#define FONT_SIZE 20
#define FPS_LIMIT 60
#define MOVEMENT_SPEED_MULTIPLIER 2
#define NUM_OF_NPCS 10
#define POINTS_WiN 100
#define NPC_ATTACK_COOLDOWN 1.0
#define PLAYER_ATTACK_COOLDOWN 5.0
#define PLAYER_HP 50

typedef enum {
  FAST,
  SLOW,
  AVERAGE,
  ENUM_COUNT,
} MonsterShape;

typedef struct {
  int up_down;
  int left_right;
} Direction;

typedef struct {
  float x;
  float y;
  int hp;
  const char *name;
  int points;
  float last_attack;
} MovableObject;

typedef struct {
  float x;
  float y;
  int hp;
  int num;
  bool is_dead;
  int last_attack;
  MonsterShape monster_shape;
} NPC;

extern int points_left;
extern float npcs_speed;
extern float player_attack_speed;

void draw_moving_text(const char *text, float x, float y,
                      MovableObject *object);
void button_movement(Direction *direction);
void draw_character_movable(MovableObject *object);
void draw_character_static(NPC *object);
void attack_circle(MovableObject *object, NPC *npcs, Direction *direction);
void move_npcs(NPC *npcs, MovableObject *target);
void current_hp(MovableObject *object);

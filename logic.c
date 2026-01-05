#include "logic.h"
#include <math.h>
#include <raylib.h>
#include <stdio.h>

int points_left = POINTS_WiN;
float npcs_speed = 1.0;
float player_attack_speed = 3.0;

void draw_character_movable(MovableObject *object) {
  Rectangle character = {object->y, object->x, 50, 50};
  DrawRectangleRec(character, GREEN);
  DrawText(object->name, character.x, character.y - FONT_SIZE, FONT_SIZE,
           YELLOW);
}

void draw_character_static(NPC *object) {
  // switch (object->monster_shape) {
  // case FAST:
  //   printf("This will be fast monster Triangle");
  // case SLOW:
  //   printf("This will be slow monster Rectangle");
  // case AVERAGE:
  //   printf("This will be average monster Circle");
  // }
  Rectangle character = {object->y, object->x, 50, 50};
  Color object_color;
  if (object->hp >= 1) {
    object_color = RED;
  } else {
    object_color = ORANGE;
  }
  DrawRectangleRec(character, object_color);
  DrawText("NPC", character.x, character.y - FONT_SIZE, FONT_SIZE, YELLOW);
}

void draw_moving_text(const char *text, float x, float y,
                      MovableObject *object) {
  DrawRectangle(y, x, MeasureText(text, FONT_SIZE), 20, RED);
  DrawText(text, y, x, FONT_SIZE, ORANGE);
}

void attack_circle(MovableObject *object, NPC *npcs, Direction *direction) {
  // if (IsKeyPressed(KEY_SPACE)) {
  // if (GetTime() - object->last_attack >= PLAYER_ATTACK_COOLDOWN) {
  if (GetTime() - object->last_attack >= player_attack_speed) {
    object->last_attack = GetTime();
    float circle_center_y = object->y + 50.0 / 2;
    float circle_center_x = object->x + 50.0 / 2;
    Vector2 circle_center = {circle_center_y, circle_center_x};
    DrawCircleLines(circle_center_y, circle_center_x, 100, BLUE);
    DrawCircleLines(circle_center_y, circle_center_x, 70, BLUE);
    DrawCircleLines(circle_center_y, circle_center_x, 30, BLUE);
    for (int i = 0; i < NUM_OF_NPCS; i++) {
      if (npcs[i].is_dead) {
        continue;
      }
      Rectangle rect = {npcs[i].y, npcs[i].x, 50, 50};
      bool is_coliison = CheckCollisionCircleRec(circle_center, 100, rect);
      if (is_coliison) {
        object->points++;
        npcs[i].hp--;
        // npcs[i].y += (npcs[i].y - 1.0) * (direction->left_right * -1);
        // npcs[i].x += (npcs[i].x - 1.0) * (direction->up_down * -1);
        float knockback_direction_y = npcs[i].y - object->y;
        float knockback_direction_x = npcs[i].x - object->x;
        float knockback_distance =
            sqrt(knockback_direction_y * knockback_direction_y +
                 knockback_direction_x * knockback_direction_x);
        npcs[i].y += (knockback_direction_y / knockback_distance) * 50.0f;
        npcs[i].x += (knockback_direction_x / knockback_distance) * 50.0f;
        if (npcs[i].hp <= 0 && !npcs[i].is_dead) {
          points_left = points_left - 10;
          npcs[i].is_dead = true;
          // npcs_speed += .5;
          player_attack_speed = player_attack_speed * 0.8;
        }
      }
    }
  }
}

void button_movement(Direction *direction) {
  switch (GetKeyPressed()) {
  case KEY_DOWN:
    direction->up_down = 1;
    break;
  case KEY_UP:
    direction->up_down = -1;
    break;
  case KEY_LEFT:
    direction->left_right = -1;
    break;
  case KEY_RIGHT:
    direction->left_right = 1;
    break;
  }
  if (IsKeyReleased(KEY_UP)) {
    direction->up_down = 0;
  }
  if (IsKeyReleased(KEY_DOWN)) {
    direction->up_down = 0;
  }
  if (IsKeyReleased(KEY_RIGHT)) {
    direction->left_right = 0;
  }
  if (IsKeyReleased(KEY_LEFT)) {
    direction->left_right = 0;
  }
}

void move_npcs(NPC *npcs, MovableObject *target) {
  for (int i = 0; i < NUM_OF_NPCS; i++) {
    if (npcs[i].is_dead) {
      continue;
    }
    float my = target->y - npcs[i].y;
    float mx = target->x - npcs[i].x;

    float distance = sqrt(my * my + mx * mx);

    if (distance >= 1) {
      npcs[i].y += (my / distance) * npcs_speed;
      npcs[i].x += (mx / distance) * npcs_speed;
    }
    if (distance <= 30) {
      if (GetTime() - npcs[i].last_attack >= NPC_ATTACK_COOLDOWN) {
        printf("PLayer is attacked\n");
        npcs[i].last_attack = GetTime();
        target->hp--;
      }
    }
  }
}

void current_hp(MovableObject *object) {
  char buffer[20];
  sprintf(buffer, "Current hp: %d", object->hp);
  DrawText(buffer, 10, 10, FONT_SIZE, RED);
}

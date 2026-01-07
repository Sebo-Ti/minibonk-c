#include "logic.h"
#include <math.h>
#include <raylib.h>
#include <stdio.h>

int points_left = POINTS_WiN;
float npcs_speed = 1.0;
float player_attack_speed = 3.0;

void draw_character_movable(MovableObject *object) {
  Rectangle character = {object->x, object->y, 50, 50};
  DrawRectangleRec(character, GREEN);
  DrawText(object->name, character.x, character.y - FONT_SIZE, FONT_SIZE,
           YELLOW);
}

void draw_character_static(NPC *object) {
  Color object_color;
  if (object->hp >= 1) {
    switch (object->monster_shape) {
    case FAST:
      object_color = PURPLE;
      break;
    case AVERAGE:
      object_color = BLUE;
      break;
    case SLOW:
      object_color = RED;
      break;
    default:
      object_color = GRAY;
      break;
    }
    // object_color = RED;
  } else {
    object_color = ORANGE;
  }
  switch (object->monster_shape) {
  case FAST: {
    Vector2 top = {object->x, object->y};
    Vector2 left = {object->x - SHAPES_SIZE, object->y + SHAPES_SIZE};
    Vector2 right = {object->x + SHAPES_SIZE, object->y + SHAPES_SIZE};
    DrawTriangle(top, left, right, object_color);
    DrawText("NPC", top.x, top.y - FONT_SIZE, FONT_SIZE, YELLOW);
    break;
  }
  case SLOW: {
    Rectangle character = {object->x, object->y, SHAPES_SIZE, SHAPES_SIZE};
    DrawRectangleRec(character, object_color);
    DrawText("NPC", character.x, character.y - FONT_SIZE, FONT_SIZE, YELLOW);
    break;
  }
  case AVERAGE: {
    Vector2 center = {object->x, object->y};
    DrawCircleV(center, SHAPES_SIZE, object_color);
    DrawText("NPC", center.x, center.y - SHAPES_SIZE - FONT_SIZE, FONT_SIZE,
             YELLOW);
    break;
  }

  default:
    break;
  }
}

void draw_moving_text(const char *text, float x, float y,
                      MovableObject *object) {
  DrawRectangle(y, x, MeasureText(text, FONT_SIZE), 20, RED);
  DrawText(text, y, x, FONT_SIZE, ORANGE);
}

void attack_circle(MovableObject *object, NPC *npcs) {
  // if (IsKeyPressed(KEY_SPACE)) {
  // if (GetTime() - object->last_attack >= PLAYER_ATTACK_COOLDOWN) {
  if (GetTime() - object->last_attack >= player_attack_speed) {
    object->last_attack = GetTime();
    float circle_center_y = object->y + 50.0 / 2;
    float circle_center_x = object->x + 50.0 / 2;
    Vector2 circle_center = {circle_center_x, circle_center_y};
    DrawCircleLines(circle_center_x, circle_center_y, 100, BLUE);
    DrawCircleLines(circle_center_x, circle_center_y, 70, BLUE);
    DrawCircleLines(circle_center_x, circle_center_y, 30, BLUE);
    for (int i = 0; i < NUM_OF_NPCS; i++) {
      if (npcs[i].is_dead) {
        continue;
      }
      Rectangle rect = {npcs[i].x, npcs[i].y, 50, 50};
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
  float speed;
  for (int i = 0; i < NUM_OF_NPCS; i++) {
    if (npcs[i].is_dead) {
      continue;
    }
    switch (npcs[i].monster_shape) {
    case FAST:
      speed = 2.0f;
      break;
    case SLOW:
      speed = 0.5f;
      break;
    case AVERAGE:
      speed = 1.0f;
      break;
    default:
      speed = 0.0f;
      break;
    }
    float my = target->y - npcs[i].y;
    float mx = target->x - npcs[i].x;

    float distance = sqrt(my * my + mx * mx);

    if (distance >= 1) {
      // npcs[i].y += (my / distance) * npcs_speed;
      // npcs[i].x += (mx / distance) * npcs_speed;
      npcs[i].y += (my / distance) * speed;
      npcs[i].x += (mx / distance) * speed;
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

ShapeCenter get_shape_center(NPC *npc) {
  ShapeCenter center;
  switch (npc->monster_shape) {
  case FAST:
    break;
  case AVERAGE:
    break;
  case SLOW:
    break;
  default:
    break;
  }
  return center;
}

void draw_projectile(Projectile *projectile) {
  if (projectile->alive)
    DrawCircle(projectile->x, projectile->y, 10, PURPLE);
}

void move_projectile(Projectile *projectile) {
  if (!projectile->alive) {
    return;
  }
  projectile->x += projectile->vx;
  projectile->y += projectile->vy;
  if (projectile->x < 0 || projectile->x > WIDTH || projectile->y < 0 ||
      projectile->y > HEIGHT) {
    projectile->alive = false;
  }
}

void shoot_projectile(MovableObject *object, Projectile *projectile) {
  float speed = 1.0f;
  if (IsKeyPressed(KEY_SPACE)) {
    projectile->y = object->y;
    projectile->x = object->x;
    printf("shooting\n");
    projectile->alive = true;
    float screen_height = (float)GetScreenHeight() / 2;
    float screen_width = (float)GetScreenWidth() / 2;
    printf("%f by %f\n", screen_height, screen_width);
    float my = screen_height - projectile->y;
    float mx = screen_width - projectile->x;
    // float my = 10.0f - projectile->y;
    // float mx = 10.0f - projectile->x;
    float distance = sqrt(my * my + mx * mx);
    projectile->vy = (my / distance) * speed;
    projectile->vx = (mx / distance) * speed;
  }
}

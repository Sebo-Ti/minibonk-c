#include "logic.h"
#include <raylib.h>
#include <stdlib.h>

NPC npcs[NUM_OF_NPCS];
Projectile fireballs[NUM_OF_NPCS];

int main(void) {
  srand(time(NULL));
  Direction direction = {0, 0};
  MovableObject object = {
      (float)WIDTH / 2, ((float)HEIGHT / 2), PLAYER_HP, "Stefan", 0, 0.0f};
  for (int i = 0; i < NUM_OF_NPCS; i++) {
    int x = rand() % WIDTH - 50;
    if (x < 0) {
      x = 20;
    }
    int y = rand() % HEIGHT - 50;
    if (y < 0) {
      y = 0;
    }
    int enemy_hp = rand() % 50 + 1;
    int random_shape = rand() % ENUM_COUNT;
    NPC enemy = {x, y, enemy_hp, i, false, 0, (MonsterShape)random_shape};
    npcs[i] = enemy;
  }

  InitWindow(WIDTH, HEIGHT, "My Bouncy Text");
  SetTargetFPS(FPS_LIMIT);
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(DARKGRAY);
    draw_character_movable(&object);
    for (int i = 0; i < NUM_OF_NPCS; i++) {
      draw_character_static(&npcs[i]);
    }
    attack_circle(&object, npcs);
    button_movement(&direction);
    object.x =
        object.x + (1 * direction.left_right) * MOVEMENT_SPEED_MULTIPLIER;
    object.y = object.y + (1 * direction.up_down) * MOVEMENT_SPEED_MULTIPLIER;
    move_npcs(npcs, &object);
    if (points_left <= 0 || object.hp <= 0) {
      EndDrawing();
      CloseWindow();
      return 0;
    }
    ClosestTarget target = get_closest_target(npcs, &object);
    shoot_projectile(&object, fireballs, target);
    move_projectile(fireballs);
    draw_projectile(fireballs);
    current_hp(&object);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}

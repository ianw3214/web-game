"use strict";

let moveSystem = {
    generate: function(speed = 100) {
        return {
            // Flags to determine what ways the entity is moving
            up: false,
            down: false,
            left: false,
            right: false,
            was_moving: false,
            // The data of the move component
            speed: speed,
        }
    },
    update: function (entities, delta) {
        for (let i in entities) {
            let entity = entities[i];
            if (entity.hasOwnProperty("move") !== true) {
                continue;
            }
            // If an entity is attacking, don't let it move
            if (entity.hasOwnProperty("attack")) {
                if (entity.attack.attacking === true) {
                    continue;
                }
            }
            let moving = false;
            let collision = null;
            if (entity.hasOwnProperty("collision")) {
                collision = entity.collision;
            }
            if (entity.hasOwnProperty("position")) {
                if (entity.move.hasOwnProperty("speed")) {
                    let distance = Math.round(entity.move.speed * delta / 1000);
                    if (entity.move.up) {
                        moving = true;
                        entity.position.y -= distance;
                        if (collision !== null) {
                            while(map.colliding(entity.position.x, entity.position.y, collision, 64)
                                || game.colliding(entity)) {
                                entity.position.y++;
                            }
                        }
                        if (entity.hasOwnProperty("animation")) {
                            entity.animation.hint = "RUN_UP";
                        }
                    }
                    if (entity.move.down) {
                        moving = true;
                        entity.position.y += distance;
                        if (collision !== null) {
                            while (map.colliding(entity.position.x, entity.position.y, collision, 64)
                                || game.colliding(entity)) {
                                entity.position.y--;
                            }
                        }
                        if (entity.hasOwnProperty("animation")) {
                            entity.animation.hint = "RUN_DOWN";
                        }
                    }
                    if (entity.move.left) {
                        moving = true;
                        entity.position.x -= distance;
                        if (collision !== null) {
                            while (map.colliding(entity.position.x, entity.position.y, collision, 64)
                                || game.colliding(entity)) {
                                entity.position.x++;
                            }
                        }
                        if (entity.hasOwnProperty("animation")) {
                            entity.animation.hint = "RUN_LEFT";
                        }
                    }
                    if (entity.move.right) {
                        moving = true;
                        entity.position.x += distance;
                        if (collision !== null) {
                            while (map.colliding(entity.position.x, entity.position.y, collision, 64)
                                || game.colliding(entity)) {
                                entity.position.x--;
                            }
                        }
                        if (entity.hasOwnProperty("animation")) {
                            entity.animation.hint = "RUN_RIGHT";
                        }
                    }
                }
            }
            if (/*entity.move.was_moving &&*/ moving === false) {
                if (entity.hasOwnProperty("animation")) {
                    entity.animation.hint = "IDLE";
                }
            }
            // Always round movement positions, otherwise it would look bad
            entity.position.x = Math.round(entity.position.x);
            entity.position.y = Math.round(entity.position.y);
            // RESET MOVEMENT AFTER HANDLING
            entity.move = {
                up: false,
                down: false,
                left: false,
                right: false,
                was_moving: moving,
                // Other data to carry along
                speed: entity.move.speed
            };
        }
    }
}
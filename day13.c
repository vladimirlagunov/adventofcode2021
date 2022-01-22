#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX(a, b) ((a) < (b) ? (b) : (a))

struct Point {
    int32_t x;
    int32_t y;
};

struct Map {
    struct Point *points;
    size_t pointNumber;
    int32_t width;
    int32_t height;
};

struct Folding {
    uint32_t position;
    enum {
        X, Y
    } along;
};

struct MapAndFoldings {
    struct Map map;
    struct Folding *foldings;
    size_t foldingNumber;
};

void freeMap(struct MapAndFoldings *map) {
    free(map->map.points);
    free(map->foldings);
}

void addReallocating(void **source, void *value, size_t elementSize, size_t *size, size_t *capacity) {
    if (*size == *capacity) {
        *capacity = ((*capacity) > 1024 ? (*capacity) + 1024 : (*capacity) * 2);
        *source = realloc(*source, *capacity * elementSize);
    }
    memcpy(*source + (*size * elementSize), value, elementSize);
    *size = (*size) + 1;
}

struct MapAndFoldings readMap(FILE *file) {
    size_t pointNumber = 0;
    size_t pointCapacity = 8;
    struct Point *points = malloc(pointCapacity * sizeof(struct Point));

    size_t foldingsNumber = 0;
    size_t foldingsCapacity = 8;
    struct Folding *foldings = malloc(foldingsCapacity * sizeof(struct Folding));

    size_t lineCapacity = 1024;
    char *line = malloc(lineCapacity);
    ssize_t read;

    // Reading points
    while (getline(&line, &lineCapacity, file) != -1) {
        struct Point point;
        char *l = line;

        if (*l == '\n') break;

        point.x = strtol(l, &l, 10);
        if (errno) {
            goto bad_line;
        }

        if (',' != *(l++)) {
            goto bad_line;
        }

        point.y = strtol(l, &l, 10);
        if (errno) {
            goto bad_line;
        }

        addReallocating(&points, &point, sizeof point, &pointNumber, &pointCapacity);
    }

    while ((read = getline(&line, &lineCapacity, file)) != -1) {
        if (read > 0 && *line == '\n') {
            break;
        }

        // "fold along "
        char *l = line + 11;
        if (l >= line + read) {
            goto bad_line;
        }

        struct Folding folding;
        if (*l == 'x') {
            folding.along = X;
        } else if (*l == 'y') {
            folding.along = Y;
        } else {
            goto bad_line;
        }

        if (*(++l) != '=') {
            goto bad_line;
        }

        folding.position = strtol(++l, NULL, 10);
        if (errno) {
            goto bad_line;
        }

        addReallocating(&foldings, &folding, sizeof folding, &foldingsNumber, &foldingsCapacity);
    }

    int32_t width = -1;
    int32_t height = -1;

    for (struct Point *p = points; p < points + pointNumber; ++p) {
        width = MAX(p->x + 1, width);
        height = MAX(p->y + 1, height);
    }

    free(line);

    return (struct MapAndFoldings) {
            .map = (struct Map) {
                    .points = realloc(points, pointNumber * sizeof(struct Point)),
                    .pointNumber = pointNumber,
                    .width = width,
                    .height = height,
            },
            .foldings = realloc(foldings, foldingsNumber * sizeof(struct Folding)),
            .foldingNumber = foldingsNumber,
    };

    bad_line:
    fprintf(stderr, "Wrong line: %s", line);
    exit(1);
}

struct MapAndFoldings readStaticMap() {
    char data[] = "6,10\n"
                  "0,14\n"
                  "9,10\n"
                  "0,3\n"
                  "10,4\n"
                  "4,11\n"
                  "6,0\n"
                  "6,12\n"
                  "4,1\n"
                  "0,13\n"
                  "10,12\n"
                  "3,4\n"
                  "3,0\n"
                  "8,4\n"
                  "1,10\n"
                  "2,14\n"
                  "8,10\n"
                  "9,0\n"
                  "\n"
                  "fold along y=7\n"
                  "fold along x=5";
    FILE *file = fmemopen(data, sizeof data, "r");
    if (file == NULL) {
        perror("fmemopen");
        exit(1);
    }
    struct MapAndFoldings result = readMap(file);
    fclose(file);
    return result;
}

void printMap(struct Map *map, struct Folding *folding) {
    char *line = malloc(map->width + 2);
    line[map->width] = '\n';
    line[map->width + 1] = '\0';
    for (uint32_t y = 0; y < map->height; ++y) {
        if (folding != NULL && folding->along == Y && folding->position == y) {
            memset(line, '-', map->width);
            fputs(line, stderr);
            continue;
        }

        memset(line, '.', map->width);
        for (struct Point *p = map->points; p < map->points + map->pointNumber; ++p) {
            if (p->y == y) {
                line[p->x] = '#';
            }
        }
        if (folding != NULL && folding->along == X) {
            line[folding->position] = '|';
        }
        fputs(line, stderr);
    }
    free(line);
}

struct Map foldMap(struct Map oldMap, struct Folding folding) {
    fprintf(stderr, "*****\nFolding along %s = %d\n*****\n", X == folding.along ? "X" : "Y", folding.position);
    printMap(&oldMap, NULL);
    fprintf(stderr, "   ===>\n");
    printMap(&oldMap, &folding);
    fprintf(stderr, "   ===>\n");

    struct Map newMap = {
            .points = malloc(oldMap.pointNumber * sizeof(struct Point)),
            .pointNumber = 0,
            .width = folding.along == X
                     ? MAX(folding.position, oldMap.width - folding.position - 1)
                     : oldMap.width,
            .height = folding.along == Y
                      ? MAX(folding.position, oldMap.height - folding.position - 1)
                      : oldMap.height,
    };

    for (struct Point *oldPoint = oldMap.points; oldPoint < oldMap.points + oldMap.pointNumber; ++oldPoint) {
        struct Point newPoint = *oldPoint;
        switch (folding.along) {
            case X:
                if (newPoint.x == folding.position) {
                    continue;
                } else if (newPoint.x < newMap.width) {
                    newPoint.x = newMap.width - (folding.position - newPoint.x);
                } else {
                    newPoint.x = newMap.width - (newPoint.x - folding.position);
                }
                break;
            case Y:
                if (newPoint.y == folding.position) {
                    continue;
                } else if (newPoint.y < newMap.height) {
                    newPoint.y = newMap.height - (folding.position - newPoint.y);
                } else {
                    newPoint.y = newMap.height - (newPoint.y - folding.position);
                }
                break;
        }

        if (newPoint.x >= newMap.width || newPoint.y >= newMap.height) {
            fprintf(stderr, "Logical error: point (%d, %d) isn't in the rect (%d, %d)\n",
                    newPoint.x, newPoint.y, newMap.width, newMap.height);
            exit(1);
        }

        uint8_t add = 1;
        for (struct Point *addedPoint = newMap.points; addedPoint < newMap.points + newMap.pointNumber; ++addedPoint) {
            if (addedPoint->x == newPoint.x && addedPoint->y == newPoint.y) {
                add = 0;
                break;
            }
        }
        if (add) {
            newMap.points[newMap.pointNumber++] = newPoint;
        }
    }

    newMap.points = realloc(newMap.points, newMap.pointNumber * sizeof(struct Point));

    printMap(&newMap, NULL);
    fputs("\n\n", stderr);
    return newMap;
}

int main(int argc, char **) {
    //struct MapAndFoldings mapAndFoldings = readStaticMap();
    FILE *f = fopen("day13_input.txt", "r");
    if (f == NULL) {
        perror("Failed to open the input file");
        exit(1);
    }
    struct MapAndFoldings mapAndFoldings = readMap(f);
    fclose(f);

    if (argc > 1) {
        // Task 1.
        mapAndFoldings.foldingNumber = 1;
    }

    struct Map map = mapAndFoldings.map;
    for (struct Folding *folding = mapAndFoldings.foldings;
         folding < mapAndFoldings.foldings + mapAndFoldings.foldingNumber;
         ++folding) {
        map = foldMap(map, *folding);
    }

    freeMap(&mapAndFoldings);

    printf("Final number of marks: %zu\n", map.pointNumber);

    return 0;
}
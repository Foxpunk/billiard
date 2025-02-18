# Бильярд на C++ с использованием SDL2

## Описание проекта

Данный проект представляет собой простую реализацию бильярдной игры для одного игрока, разработанную на языке C++ с использованием библиотеки SDL2 версии. В игре реализованы базовые механики, такие как удар по битку, забивание шаров в лузы и физика их взаимодействия (отталкивание друг от друга). Цель игры — забить все шары в лузы.

## Зависимости

Для корректной работы проекта необходимо установить следующие библиотеки:

- **SDL2 2.30.2**: Основная библиотека для работы с графикой, аудио и вводом.
- **SDL2_image 2.8.2**: Дополнительная библиотека для загрузки различных форматов изображений.
- **SDL2_ttf 2.22.0**: Библиотека для работы с TrueType шрифтами.

Ссылки для скачивания:

- [SDL2 2.30.2](https://github.com/libsdl-org/SDL/releases/tag/release-2.30.2)
- [SDL2_image 2.8.2](https://github.com/libsdl-org/SDL_image/releases/tag/release-2.8.2)
- [SDL2_ttf 2.22.0](https://github.com/libsdl-org/SDL_ttf/releases/tag/release-2.22.0)

## Установка и сборка

   **Настройте проект в Visual Studio**:
   - Создайте новый проект C++ в Visual Studio.
   - Добавьте исходные файлы проекта в ваш проект.
   - В настройках проекта укажите пути к заголовочным файлам и библиотекам SDL2, SDL2_image и SDL2_ttf.

## Структура проекта

Проект состоит из следующих основных классов:

### 1. Класс `Vector2D`

Класс для работы с двумерными векторами.

#### Поля:
- `float x` — координата X вектора.
- `float y` — координата Y вектора.

#### Методы:
- `Vector2D()` — конструктор по умолчанию, инициализирует вектор с координатами (0, 0).
- `Vector2D(float x, float y)` — конструктор, инициализирует вектор с заданными координатами.
- `float getX() const` — метод для получения координаты X.
- `float getY() const` — метод для получения координаты Y.
- `void setX(float x)` — метод для установки координаты X.
- `void setY(float y)` — метод для установки координаты Y.
- `float dot(const Vector2D& other) const` — метод для вычисления скалярного произведения с другим вектором.
- `void normalize()` — метод для нормализации вектора.
- `float length() const` — метод для вычисления длины вектора.
- `Vector2D operator+(const Vector2D& other) const` — оператор сложения векторов.
- `Vector2D operator-(const Vector2D& other) const` — оператор вычитания векторов.
- `Vector2D operator*(float scalar) const` — оператор умножения вектора на скаляр.

### 2. Класс `Wall`

Класс, представляющий стену стола.

#### Поля:
- `Vector2D start` — начальная точка стены.
- `Vector2D end` — конечная точка стены.

#### Методы:
- `Wall(const Vector2D& start, const Vector2D& end)` — конструктор, инициализирующий стену с заданными начальной и конечной точками.
- `Vector2D getStart() const` — метод для получения начальной точки стены.
- `Vector2D getEnd() const` — метод для получения конечной точки стены.

### 3. Класс `Pocket`

Класс, представляющий лузу на столе.

#### Поля:
- `Vector2D position` — позиция лузы.
- `float radius` — радиус лузы.

#### Методы:
- `Pocket(float x, float y, float r)` — конструктор, инициализирующий лузу с заданными координатами и радиусом.
- `Vector2D getPosition() const` — метод для получения позиции лузы.
- `float getRadius() const` — метод для получения радиуса лузы.
- `void draw(SDL_Renderer* renderer) const` — метод для отрисовки лузы с использованием SDL.

### 4. Класс `Player`

Класс, представляющий игрока.

#### Поля:
- `int score` — количество очков игрока.
- `SDL_Renderer* renderer` — указатель на рендерер SDL.
- `TTF_Font* font` — указатель на шрифт для отображения очков.

#### Методы:
- `Player(SDL_Renderer* renderer, TTF_Font* font)` — конструктор, инициализирующий игрока с заданным рендерером и шрифтом.
- `~Player()` — деструктор.
- `int getScore() const` — метод для получения текущего количества очков.
- `void addScore(int points)` — метод для добавления очков.
- `void draw(SDL_Renderer* renderer)` — метод для отрисовки очков игрока на экране.

### 5. Класс `Ball`

Класс, представляющий шар.

#### Поля:
- `Vector2D position` — позиция шара.
- `Vector2D texPos` — позиция текстуры шара.
- `Vector2D velocity` — скорость шара.
- `float radius` — радиус шара.
- `int id` — идентификатор шара.
- `float mass` — масса шара (по умолчанию 100).
- `bool isPocketed` — флаг, указывающий, забит ли шар в лузу.
- `SDL_Renderer* renderer` — указатель на рендерер SDL.
- `SDL_Texture* texture` — указатель на текстуру шара.

#### Методы:
- `Ball(float posx, float posy, float r, int ID, SDL_Renderer* renderer, const char* texturePath = nullptr)` — конструктор, инициализирующий шар с заданными параметрами.
- `void printID()` — метод для вывода идентификатора шара.
- `void printBall()` — метод для вывода информации о шаре.
- `void draw()` — метод для отрисовки шара.
- `void update()` — метод для обновления позиции шара.
- `void updatePhysics()` — метод для обновления физики шара.
- `void resetCue()` — метод для сброса позиции битка.
- `bool isMoving() const` — метод для проверки, движется ли шар.

### 6. Класс `Cue`

Класс, представляющий кий.

#### Поля:
- `SDL_Texture* texture` — указатель на текстуру кия.
- `SDL_Renderer* renderer` — указатель на рендерер SDL.
- `float mouseAngle` — угол поворота кия.
- `bool dragging` — флаг, указывающий, происходит ли перетаскивание кия.
- `Uint32 dragStartTime` — время начала перетаскивания.
- `float power` — сила удара.
- `const float maxSpeed` — максимальная скорость удара (по умолчанию 100.0f).

#### Методы:
- `Cue(SDL_Renderer* renderer, const char* texturePath)` — конструктор, инициализирующий кий с заданным рендерером и текстурой.
- `~Cue()` — деструктор.
- `void handleInput(const SDL_Event& event, Vector2D& cueBallPosition, Vector2D& cueBallVelocity)` — метод для обработки ввода.
- `void draw(SDL_Renderer* renderer, const Vector2D& cueBallPosition, bool ballsAreMoving, float mouseAngle)` — метод для отрисовки кия.
- `float getAngle() const` — метод для получения угла поворота кия.
- `float getPower() const` — метод для получения силы удара.
- `void reset()` — метод для сброса состояния кия.

### 7. Класс `Table`

Класс, представляющий бильярдный стол.

#### Поля:
- `float width` — ширина стола.
- `float height` — высота стола.
- `SDL_Texture* texture` — указатель на текстуру стола.
- `std::vector<Pocket> pockets` — список луз.
- `std::vector<Wall> walls` — список стен.
- `SDL_Renderer* renderer` — указатель на рендерер SDL.

#### Методы:
- `Table(float width, float height, const char* texturePath, SDL_Renderer* renderer)` — конструктор, инициализирующий стол с заданными параметрами.
- `~Table()` — деструктор.
- `void init()` — метод для инициализации стола, добавления луз и стен.
- `void draw(SDL_Renderer* renderer)` — метод для отрисовки стола.
- `void addPocket(const Pocket& pocket)` — метод для добавления лузы.
- `void addWall(const Wall& wall)` — метод для добавления стены.
- `std::vector<Pocket>& getPockets()` — метод для получения списка луз.
- `std::vector<Wall>& getWalls()` — метод для получения списка стен.
- `float getWidth()` — метод для получения ширины стола.
- `float getHeight()` — метод для получения высоты стола.

### 8. Класс `Physics`

Класс, отвечающий за физику взаимодействий.

#### Методы:
- `static void HandleCollisions(Table& table, std::vector<Ball*>& balls)` — метод для обработки всех столкновений в игре.
- `static bool DetectCollision(Ball* b1, Ball* b2)` — метод для обнаружения столкновения двух шаров.
- `static bool DetectCollisionHole(Ball* b, Pocket* h)` — метод для обнаружения столкновения шара с лузой.
- `static Vector2D* DetectCollisionEdge(Ball* b, Wall* e)` — метод для обнаружения столкновения шара со стеной.
- `static void DynamicCollision(Ball* b1, Ball* b2)` — метод для обработки динамического столкновения двух шаров.

### 9. Класс `Game`

Основной класс игры.

#### Поля:
- `bool isRunning` — флаг, указывающий, запущена ли игра.
- `SDL_Window* window` — указатель на окно SDL.
- `SDL_Renderer* renderer` — указатель на рендерер SDL.
- `TTF_Font* font` — указатель на шрифт.
- `Table* table` — указатель на объект стола.
- `Player* player` — указатель на объект игрока.
- `Cue* cue` — указатель на объект кия.
- `std::vector<Ball*> balls` — список шаров.

#### Методы:
- `Game()` — конструктор, инициализирующий игру.
- `~Game()` — деструктор.
- `bool init(const char* title, int width, int height, bool fullscreen)` — метод для инициализации игры.
- `void handleEvents()` — метод для обработки событий.
- `void update()` — метод для обновления состояния игры.
- `void render()` — метод для отрисовки игры.
- `void clean()` — метод для очистки ресурсов.

---

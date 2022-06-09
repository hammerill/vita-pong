#include <SDL.h>

#include <psp2/kernel/processmgr.h>
#include <psp2/ctrl.h>
#include <psp2/touch.h>

enum
{
    SCREEN_WIDTH = 960,
    SCREEN_HEIGHT = 544
};

void addPos(int *pos, int add, int size, int screenSize)
{
    if (*pos + add < 0)
    {
        *pos = 0;
    }
    else if (*pos + add + size > screenSize)
    {
        *pos = screenSize - size;
    }
    else
    {
        *pos = *pos + add;
    }
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
    SceCtrlData ctrl;

    sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, SCE_TOUCH_SAMPLING_STATE_START);
    sceTouchEnableTouchForce(SCE_TOUCH_PORT_FRONT);
    SceTouchData touch[SCE_TOUCH_PORT_MAX_NUM];

    const int pong_w = SCREEN_WIDTH / 40;
    const int pong_h = SCREEN_HEIGHT / 4;
    const int ball_w = SCREEN_WIDTH / 20;

    SDL_Rect player1 = {0, (SCREEN_HEIGHT / 2) - (pong_h / 2), pong_w, pong_h};
    SDL_Rect player2 = {SCREEN_WIDTH - pong_w, (SCREEN_HEIGHT / 2) - (pong_h / 2), pong_w, pong_h};

    SDL_Rect ball = {(SCREEN_WIDTH / 2) - (ball_w / 2), (SCREEN_HEIGHT / 2) - (ball_w / 2), ball_w, ball_w};

    const int moveSpeed = 10;
    const int stickDeadZone = 10;
    const int stickCenter = 128;
    const int stickMax = stickCenter;

    int ballIx = rand() % 2 ? moveSpeed : -moveSpeed;
    int ballIy = rand() % 2 ? moveSpeed : -moveSpeed;

    bool isRunning = true;
    while (isRunning)
    {
        sceCtrlPeekBufferPositive(0, &ctrl, 1);
        sceTouchPeek(0, &touch[0], 1);

        if (ctrl.ly > stickCenter + stickDeadZone || ctrl.ly < stickCenter - stickDeadZone)
        {
            addPos(&player1.y, (ctrl.ly - stickCenter) / (stickMax / moveSpeed), player1.h, SCREEN_HEIGHT);
        }
        if (ctrl.ry > stickCenter + stickDeadZone || ctrl.ry < stickCenter - stickDeadZone)
        {
            addPos(&player2.y, (ctrl.ry - stickCenter) / (stickMax / moveSpeed), player2.h, SCREEN_HEIGHT);
        }

        if (ball.x + ball_w >= SCREEN_WIDTH || ball.x <= 0)
        {
            ballIx = rand() % 2 ? moveSpeed : -moveSpeed;
            ballIy = rand() % 2 ? moveSpeed : -moveSpeed;
            ball.x = (SCREEN_WIDTH / 2) - (ball_w / 2);
            ball.y = (SCREEN_HEIGHT / 2) - (ball_w / 2);
        }
        if (ball.y + ball_w >= SCREEN_HEIGHT || ball.y <= 0)
        {
            ballIy = -ballIy;
        }

        if (ball.x <= pong_w && ball.y + ball_w >= player1.y && ball.y <= player1.y + pong_h ||
            ball.x + ball_w > SCREEN_WIDTH - pong_w && ball.y + ball_w >= player2.y && ball.y <= player2.y + pong_h)
        {
            ballIx = -ballIx;
            ballIy = rand() % 2 ? moveSpeed : -moveSpeed;
        }

        addPos(&ball.x, ballIx, ball.w, SCREEN_WIDTH);
        addPos(&ball.y, ballIy, ball.h, SCREEN_HEIGHT);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderFillRect(renderer, &player1);
        SDL_RenderFillRect(renderer, &player2);
        SDL_RenderFillRect(renderer, &ball);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    sceKernelExitProcess(0);
    return 0;
}
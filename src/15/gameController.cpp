#include "../../SDLTemplateCode/include/SDLTemplate.hpp"

#define FPS 165     // 每秒钟帧数

// 算圆的面积
#define GET_CIRCLE_AREA(radius) ((M_PI) * (radius) * (radius))

// 算当前摇杆位置与摇杆原点的距离
#define GET_ROKER_DISTANCE(rokerPos) (                                  \
    std::sqrt(std::pow(rokerPos.x, 2) + std::pow(rokerPos.y , 2))    \
)

/**
 * @brief 根据摇杆所处的位置，调整纹理移动的速度。
 *  
 * @param __speed               速度值
 * @param __currentRokerPos     以当前摇杆位置与摇杆原点为半径所构成的圆的面积
 * @param __speedShown          需要显示的速度信息
 * @param __render              渲染器
*/
void speedControl(
    int & __speed, const float __currentRokerPos, 
    FontsTexture & __speedShown, SDL_Renderer * __render
);

int WinMain(int argc, char const *argv[])
{
    SystemInit::WindowSize windowSize = {1800, 1000};
    SystemInit sysInit(windowSize, "15 SDL Game Controller");

    EventsControl events;

    sysInit.init();

    Uint64 startTick = SDL_GetTicks64();
    Uint64 renderTick = STANDING_RENDER_TIME(startTick, FPS);

    FontsTexture rokersPosShown;
    FontsTexture arrowAngleShown;
    FontsTexture speedShown;
    TextureImage arrow;

    rokersPosShown.open("../fonts/Consolas-Regular.ttf", 20);
    arrowAngleShown.open("../fonts/Consolas-Regular.ttf", 20);
    speedShown.open("../fonts/Consolas-Regular.ttf", 20);
    arrow.load("../img/12/arrow.png", {0, 0xFF, 0XFF, 0XFF}, SDL_TRUE, sysInit.getRenderer());

    SDL_Point arrowPos = {50, 50};

    int speed = 0;      // 起始速度

    TextureImage::FilpAttribution flip = {
        0, {arrow.getRenderPosition().w / 2, arrow.getRenderPosition().h / 2}, SDL_FLIP_HORIZONTAL};

    SDL_Point rokersPosShownRenderPos = {0, 0};

    while (!events.getRunstate())
    {
        events.recordEvents();

        SDL_RenderClear(sysInit.getRenderer());
        SDL_SetRenderDrawColor(sysInit.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);

        arrow.render(
            arrowPos.x, arrowPos.y,
            sysInit.getRenderer(), arrow.defaultClip(), flip
        );

        rokersPosShown.render(
                rokersPosShownRenderPos.x, rokersPosShownRenderPos.y,
                sysInit.getRenderer()
        );

        arrowAngleShown.render(0, 30, sysInit.getRenderer());

        speedShown.render(0, 65, sysInit.getRenderer());

#if true
        if (events.getRockersPosition().size() != 0)
        {
            rokersPosShown.load(
                events.getRockersPosition()[0].show(),
                {0, 0, 0, 0xFF}, sysInit.getRenderer()
            );

            arrowAngleShown.load(
                "Arrow angle: " + std::to_string(std::abs(flip.angle)) + 
                " Arrow Position: (" + std::to_string(arrowPos.x) + ", " + std::to_string(arrowPos.y) + ")",
                {0, 0, 0, 0xFF}, sysInit.getRenderer()
            );

            SDL_FPoint leftRokerPos = events.getRockersPosition()[0].leftRokerPos;
            float currentRokerRange = GET_CIRCLE_AREA(GET_ROKER_DISTANCE(leftRokerPos));
            
            speedControl(speed, std::abs(currentRokerRange), speedShown, sysInit.getRenderer());

            arrowPos.x += leftRokerPos.x * speed;
            arrowPos.y -= leftRokerPos.y * speed;

            flip.angle = -std::atan2(leftRokerPos.y, leftRokerPos.x) * (180.0F / M_PI);

            // 调整角度到 0 到 360 度之间
            if (flip.angle < 0.0F) { flip.angle += 360.0F; }

            if (arrowPos.x < 0) { arrowPos.x = 0; }
            if (arrowPos.x > (windowSize.w - arrow.getRenderPosition().w)) { 
                arrowPos.x = windowSize.w - arrow.getRenderPosition().w; 
            }
            
            if (arrowPos.y < 0) { arrowPos.y = 0; }
            if (arrowPos.y > (windowSize.h - arrow.getRenderPosition().h)) {
                arrowPos.y = windowSize.h - arrow.getRenderPosition().h;
            }
        }
        else 
        {
            rokersPosShown.load(
                "No Game Controller Connected.....",
                {0, 0, 0, 0xFF}, sysInit.getRenderer()
            );
        }
#endif
        SDL_RenderPresent(sysInit.getRenderer());
        frameControl(startTick, renderTick);
    }

    return EXIT_SUCCESS;
}

void speedControl(
    int & __speed, const float __currentRokerPos, 
    FontsTexture & __speedShown, SDL_Renderer * __render
)
{
    const float slowRokerRange      = GET_CIRCLE_AREA(0.45F);
    const float mediumRokerRange    = GET_CIRCLE_AREA(0.85F);
    const float fastRokerRange      = GET_CIRCLE_AREA(1.00F);

    if ((__currentRokerPos == 0.0F))
    {
        __speed = 0;
        __speedShown.load(
            "[STOP] = " + std::to_string(__speed) + " Px / Frame", 
            {0, 0, 0, 0xFF}, __render
        );
    }
    if ((__currentRokerPos > 0.0F) && (__currentRokerPos <= slowRokerRange)) { 
        __speed = 3;
        __speedShown.load(
            "[LOW SPEED] = " + std::to_string(__speed) + " Px / Frame", 
                {0, 0, 0, 0xFF}, __render
            );
    }
    if ((__currentRokerPos > slowRokerRange) && (__currentRokerPos <= mediumRokerRange)) {
        __speed = 5;
        __speedShown.load(
            "[MEDIUM SPEED] = " + std::to_string(__speed) + " Px / Frame", 
            {0, 0, 0, 0xFF}, __render
        );
    }
    if ((__currentRokerPos > mediumRokerRange) && (__currentRokerPos <= fastRokerRange)) { 
        __speed = 7;
        __speedShown.load(
            "[HEIGH SPEED] = " + std::to_string(__speed) + " Px / Frame", 
            {0, 0, 0, 0xFF}, __render
        );
    }
}
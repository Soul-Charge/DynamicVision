/*\
|*| 编译环境：VS2019
|*| TODO:
\*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <easyx.h>
#include <windows.h>
#include <pthread.h>
#define TITLESIZE 80
#define DELAY 1
#define MAXSPEED (ballRadius / 2)
#define MINSPEED 1
#define MAXRADIUS 30
#define MINRADIUS 1
#define KEY_DOWN(vk_c)(GetAsyncKeyState(vk_c)&0x8000)

unsigned short GetUShortVal(void);
void DrawBall(int x, int y, unsigned short ballRadius);
void WipeBall(int x, int y, unsigned short ballRadius);
void IfTouchBorder(int x, int y);
void IfOverBorder(int x, int y);
void* ReactKeyDown(void* pv);
void ChangeTrack(void);
void DrawTextCenter(LPCTSTR string);
int Rand(int m, int n);

unsigned short windowWidth = 1367; //窗口宽
unsigned short windowHeight = 697; //窗口高
unsigned short ballRadius = 10;    //小球半径
short addX = 2;                    //x轴上的速度
short addY = 1;                    //y轴上的速度
/* 在小球要超出边框时使用的附属addX, addY */
short addX_aux = 0;
short addY_aux = 0; 

int main(void)
{    /* 初始化小球位置 */
    int x = windowWidth / 2, y = windowHeight / 2;
    srand((unsigned int)time(0));

    /* 获取窗口宽高和小球半径 */
    printf("输入窗口宽：");
    windowWidth = GetUShortVal();
    printf("输入窗口高：");
    windowHeight = GetUShortVal();
    printf("输入小球半径：");
    ballRadius = GetUShortVal();

    /* 窗口实际显示区大小为0,0 到 windowWidth-1, windowHeight-1 */
    initgraph(windowWidth, windowHeight);

    /* 创建一个线程响应键盘消息 */
    pthread_t t;
    if (pthread_create(&t, NULL, ReactKeyDown, NULL) == -1)
    {
        fprintf(stderr, "创建线程失败\n");
        system("pause");
    }

    /* 绘制和移动小球 */
    while (1)
    {
        DrawBall(x, y, ballRadius);
        Sleep(DELAY);
        IfTouchBorder(x, y);
        IfOverBorder(x, y);
        WipeBall(x, y, ballRadius);
        if (addX_aux != 0)
            x += addX_aux;
        else
            x += addX;
        if (addY_aux != 0)
            y += addY_aux;
        else
            y += addY;
    }
    return 0;
}

/* 获取输入数值(unsigned short int) */
unsigned short GetUShortVal(void)
{
    unsigned short val;
    char ch;

    while (scanf_s("%hu", &val) == 0)
    {
        putchar('"');
        while ((ch = getchar()) != '\n')
            putchar(ch);
        putchar('"');
        puts("is not a valid input.");
        printf("Please enter a number, such as 2, 18, or 3: ");
    }
    /* 清除输入产生的换行符 */
    while (getchar() != '\n')
        continue;
    return val;
}

/* 绘制小球 */
void DrawBall(int x, int y, unsigned short ballRadius)
{
    setlinecolor(WHITE);
    setfillcolor(WHITE);
    fillcircle(x, y, ballRadius);
}

/* 擦除小球 */
void WipeBall(int x, int y, unsigned short ballRadius)
{
    setlinecolor(BLACK);
    setfillcolor(BLACK);
    fillcircle(x, y, ballRadius + 3); // +3是防止小球缩小时留下尾迹的简单粗暴的方法
}

/* 根据碰壁位置进行速度方向的修改 */
void IfTouchBorder(int x, int y)
{
    if (x - ballRadius == 0 && y - ballRadius == 0) //左上角
    {
        addX = -addX;
        addY = -addY;
    }
    else if (x + ballRadius == windowWidth - 1 && y - ballRadius == 0) //右上角
    {
        addX = -addX;
        addY = -addY;
    }
    else if (x - ballRadius == 0 && y + ballRadius == windowHeight - 1) //左下角
    {
        addX = -addX;
        addY = -addY;
    }
    else if (x + ballRadius == windowWidth - 1 && y + ballRadius == windowHeight - 1) //右下角
    {
        addX = -addX;
        addY = -addY;
    }
    else if (y - ballRadius == 0) //上
        addY = -addY;
    else if (y + ballRadius == windowHeight - 1) //下
        addY = -addY;
    else if (x - ballRadius == 0) //左
        addX = -addX;
    else if (x + ballRadius == windowWidth - 1) //右
        addX = -addX;
}

 /* 检测是否超出边框 */
void IfOverBorder(int x, int y)
{
    if (y - ballRadius + addY < 0) // 上
        addY_aux= -abs(y - ballRadius);
    else if (y + ballRadius + addY > windowHeight - 1) // 下
        addY_aux = (windowHeight - 1) - (y + ballRadius);
    else if (x - ballRadius + addX < 0) // 左
        addX_aux = -abs(x - ballRadius);
    else if (x + ballRadius + addX > windowWidth - 1) // 右
        addX_aux = (windowWidth - 1) - (x + ballRadius);
    else
    {
        addX_aux = 0;
        addY_aux = 0;
    }
}

/* 响应键盘消息（线程函数） */
void* ReactKeyDown(void* pv)
{
    while (1)
    {
        /* 按下方向键上并且 addX 和 addY 都小于最大值 */
        /* addX 和 addY 进行比较运算要先取绝对值，因为此处正负代表方向 */
        /* 同时addX 和 addY 的加减也要根据方向，加速：负的减，正的加，减速则相反 */
        /* 加速 */
        if (KEY_DOWN(VK_UP) && abs(addX) < MAXSPEED && abs(addY) < MAXSPEED)
        {
            if (addX < 0)
                addX--;
            else
                addX++;
            if (addY < 0)
                addY--;
            else
                addY++;
            DrawTextCenter(_T("SPEED UP"));
        }
        /* 按下方向键下并且 addX 和 addY 都大或等于于最小值 */
        /* 减速 */
        else if (KEY_DOWN(VK_DOWN) && abs(addX) > MINSPEED && abs(addY) > MINSPEED)
        {
            if (addX < 0)
                addX++;
            else
                addX--;
            if (addY < 0)
                addY++;
            else
                addY--;
            DrawTextCenter(_T("SPEED DOWN"));
        }
        /* 按下方向键左并且小球半径小于或等于最大值 */
        else if (KEY_DOWN(VK_LEFT) && ballRadius <= MAXRADIUS)
        {
            ballRadius++;
            DrawTextCenter(_T("RADIUS +"));
        }
        /* 按下方向键右并且小球半径大于或等于最小值 */
        else if (KEY_DOWN(VK_RIGHT) && ballRadius >= MINRADIUS)
        {
            ballRadius--;
            DrawTextCenter(_T("RADIUS -"));
        }
        /* 按下空格键转换小球运动轨迹 */
        else if (KEY_DOWN(VK_SPACE))
        {
            ChangeTrack();
            DrawTextCenter(_T("Change Track"));
        }
        Sleep(100);
    }
    return NULL;
} 

/* 在窗口中央绘制文字，用法：DrawText(_T("string") */
void DrawTextCenter(LPCTSTR textString)
{
    RECT textArea = { 0, 0, windowWidth, windowHeight };
    /* 设置字体为高32像素的白色宋体 */
    settextstyle(32, 0, _T("宋体"));
    settextcolor(WHITE);
    drawtext(textString, &textArea, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    Sleep(100);
    settextcolor(BLACK);
    drawtext(textString, &textArea, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    return;
}

/* 改变小球运动轨迹 */
void ChangeTrack(void)
{
    /* addY 则为速度总和减去新 addX */
    /* 考虑正负代表方向，使用绝对值 */
    int speedSum = abs(addX) + abs(addY);
    if (speedSum > MAXSPEED)
    {
        /* 确保 addY 不会等于0，限制 addX 的范围为[-MAXSPEED,MAXSPEED] */
        addX = Rand(-MAXSPEED, MAXSPEED);
        addY = speedSum - abs(addX);
    }
    else
    {
        /* 确保 addY 不会等于0，限制 addX 的范围为[speedSum + 1,speedSum - 1] */
        addX = Rand(-speedSum + 1, speedSum - 1);
        addY = speedSum - abs(addX);
    }
}

/* 生成 m 到 n (包含 m 和 n ）之间的随机数 */
/* 调用方式：Rand(下界, 上界); */
int Rand(int m, int n)
{
    return rand() % (n - m + 1) + m;
}

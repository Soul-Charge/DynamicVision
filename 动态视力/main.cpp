/*\
|*| ���뻷����VS2019
|*| TODO:
|*| 1. �����ס�������һֱ��ı�С��켣������
|*| 2. ���С�����Խ���߽������
|*| 3. ������ٻ�ʹaddY���0������
\*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <easyx.h>
#include <windows.h>
#include <pthread.h>
#define TITLESIZE 80
#define DELAY 1
#define MAXSPEED 10a
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
void DrawText(LPCTSTR string);

unsigned short windowWidth = 1367; //���ڿ�
unsigned short windowHeight = 697; //���ڸ�
unsigned short ballRadius = 10;    //С��뾶
short addX = 2;                    //x���ϵ��ٶ�
short addY = 1;                    //y���ϵ��ٶ�
/* ��С��Ҫ�����߿�ʱʹ�õĸ���addX, addY */
short addX_aux = 0;
short addY_aux = 0; 

int main(void)
{    /* ��ʼ��С��λ�� */
    int x = windowWidth / 2, y = windowHeight / 2;

    /* ��ȡ���ڿ�ߺ�С��뾶 */
    printf("���봰�ڿ�");
    windowWidth = GetUShortVal();
    printf("���봰�ڸߣ�");
    windowHeight = GetUShortVal();
    printf("����С��뾶��");
    ballRadius = GetUShortVal();

    /* ����ʵ����ʾ����СΪ0,0 �� windowWidth-1, windowHeight-1 */
    initgraph(windowWidth, windowHeight);

    /* ����һ���߳���Ӧ������Ϣ */
    pthread_t t;
    if (pthread_create(&t, NULL, ReactKeyDown, NULL) == -1)
    {
        fprintf(stderr, "�����߳�ʧ��\n");
        system("pause");
    }

    /* ���ƺ��ƶ�С�� */
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

/* ��ȡ������ֵ(unsigned short int) */
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
    /* �����������Ļ��з� */
    while (getchar() != '\n')
        continue;
    return val;
}

/* ����С�� */
void DrawBall(int x, int y, unsigned short ballRadius)
{
    setlinecolor(WHITE);
    setfillcolor(WHITE);
    fillcircle(x, y, ballRadius);
}

/* ����С�� */
void WipeBall(int x, int y, unsigned short ballRadius)
{
    setlinecolor(BLACK);
    setfillcolor(BLACK);
    fillcircle(x, y, ballRadius+3); // +3�Ƿ�ֹС����Сʱ����β���ļ򵥴ֱ��ķ���
}

/* ��������λ�ý����ٶȷ�����޸� */
void IfTouchBorder(int x, int y)
{
    if (x - ballRadius == 0 && y - ballRadius == 0) //���Ͻ�
    {
        addX = -addX;
        addY = -addY;
    }
    else if (x + ballRadius == windowWidth - 1 && y - ballRadius == 0) //���Ͻ�
    {
        addX = -addX;
        addY = -addY;
    }
    else if (x - ballRadius == 0 && y + ballRadius == windowHeight - 1) //���½�
    {
        addX = -addX;
        addY = -addY;
    }
    else if (x + ballRadius == windowWidth - 1 && y + ballRadius == windowHeight - 1) //���½�
    {
        addX = -addX;
        addY = -addY;
    }
    else if (y - ballRadius == 0) //��
        addY = -addY;
    else if (y + ballRadius == windowHeight - 1) //��
        addY = -addY;
    else if (x - ballRadius == 0) //��
        addX = -addX;
    else if (x + ballRadius == windowWidth - 1) //��
        addX = -addX;
}

 /* ����Ƿ񳬳��߿� */
void IfOverBorder(int x, int y)
{
    if (y - ballRadius + addY < 0 && y != ballRadius) // ��
        addY_aux= y - ballRadius;
    else if (y + ballRadius + addY > windowHeight - 1 && y != windowHeight - ballRadius) // ��
        addY_aux = (windowHeight - 1) - (y + ballRadius);
    else if (x - ballRadius + addX < 0 && x != ballRadius) // ��
        addX_aux = x - ballRadius;
    else if (x + ballRadius + addX > windowWidth - 1 && x != windowWidth - ballRadius) // ��
        addX_aux = (windowWidth - 1) - (x + ballRadius);
    else
    {
        addX_aux = 0;
        addY_aux = 0;
    }
}

/* ��Ӧ������Ϣ���̺߳����� */
void* ReactKeyDown(void* pv)
{
    while (1)
    {
        /* ���·�����ϲ��� addX �� addY ��С�ڻ�������ֵ */
        /* addX �� addY ���бȽ�����Ҫ��ȡ����ֵ����Ϊ�˴����������� */
        if (KEY_DOWN(VK_UP) && abs(addX) <= MAXSPEED && abs(addY) <= MAXSPEED)
        {
            addX++;
            addY++;
            DrawText(_T("SPEED UP"));
        }
        /* ���·�����²��� addX �� addY ������������Сֵ */
        else if (KEY_DOWN(VK_DOWN) && abs(addX) >= MINSPEED && abs(addY) >= MINSPEED)
        {
            addX--;
            addY--;
            DrawText(_T("SPEED DOWN"));
        }
        /* ���·��������С��뾶С�ڻ�������ֵ */
        else if (KEY_DOWN(VK_LEFT) && ballRadius <= MAXRADIUS)
        {
            ballRadius++;
            DrawText(_T("RADIUS +"));
        }
        /* ���·�����Ҳ���С��뾶���ڻ������Сֵ */
        else if (KEY_DOWN(VK_RIGHT) && ballRadius >= MINRADIUS)
        {
            ballRadius--;
            DrawText(_T("RADIUS -"));
        }
        Sleep(100);
    }
    return NULL;
} 

/* �ڴ�������������֣��÷���DrawText(_T("string") */
void DrawText(LPCTSTR textString)
{
    RECT textArea = { 0, 0, windowWidth, windowHeight };
    /* ��������Ϊ��32���صİ�ɫ���� */
    settextstyle(32, 0, _T("����"));
    settextcolor(WHITE);
    drawtext(textString, &textArea, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    Sleep(100);
    settextcolor(BLACK);
    drawtext(textString, &textArea, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    return;
}
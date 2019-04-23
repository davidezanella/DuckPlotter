#include "AFMotor.h"
#include <math.h>

void moveLinear(AF_Stepper motorX, AF_Stepper motorY, float fromX, float fromY, float toX, float toY)
{
    //find the equation of the line (y = mx + q)
    float m = (toY - fromY) / (toX / fromX);
    float q = fromY - (m * fromX);

    float x = fromX;
    float y = fromY;

    while (x != toX)
    {
        x++;
        float nextY = m * x + q;
        //move motors
    }
}

void moveArc(AF_Stepper motorX, AF_Stepper motorY, float fromX, float fromY, float toX, float toY, float radius, float centerX, float centerY, bool clockwise)
{
    // (x-a)^2 + (y-b)^2 = r^2 where a is centerX, b is centerY and r is the radius
    // y^2 + (-2b)y + ((x-a)^2 + b^2 - r^2) = 0

    float a = 1;
    float b = -2 * centerY;
    float x = fromX;
    float y = fromY;
    int incr = (clockwise ? 1 : -1);

    while (x != toX)
    {
        x += incr;
        float c = pow(x - centerX, 2) + pow(centerY, 2) - pow(radius, 2);
        float discriminant = b * b - 4 * a * c;
        float y1 = (-b + sqrt(discriminant)) / (2 * a);
        float y2 = (-b - sqrt(discriminant)) / (2 * a);
        float nextY;
        //choose the right y
        if (pow(y1 - y, 2) < pow(y2 - y, 2))
        {
            nextY = y1;
        }
        else
        {
            nextY = y2;
        }

        //move motors
    }
}

void movePen(bool down)
{
}

void moveArc(AF_Stepper motorX, AF_Stepper motorY, float fromX, float fromY, float toX, float toY, float offsetX, float offsetY, bool clockwise)
{
    //find the radius
    float radius = sqrt(pow(offsetX, 2) + pow(offsetY, 2));
    float centerX = toX + offsetX;
    float centerY = toY + offsetY;

    moveArc(motorX, motorY, fromX, fromY, toX, toY, radius, centerX, centerY, clockwise);
}

void moveArc(AF_Stepper motorX, AF_Stepper motorY, float fromX, float fromY, float toX, float toY, float radius, bool clockwise)
{
    //find the centerX and centerY
    float radsq = radius * radius;
    float q = sqrt(pow(fromX - toX, 2) + pow(fromY - toY, 2));
    float x3 = (fromX + toX) / 2;
    float centerX = x3 + sqrt(radsq - pow(q / 2, 2) * ((fromY - toY) / q));

    float y3 = (fromY + toY) / 2;
    float centerY = y3 + sqrt(radsq - pow(q / 2, 2) * ((fromX - toX) / q));

    moveArc(motorX, motorY, fromX, fromY, toX, toY, radius, centerX, centerY, clockwise);
}
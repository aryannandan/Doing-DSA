class Solution {
public:
    bool checkOverlap(int radius, int xCenter, int yCenter,
                      int x1, int y1, int x2, int y2) {

        // Find the closest x-coordinate in the rectangle
        int closestX = max(x1, min(xCenter, x2));

        // Find the closest y-coordinate in the rectangle
        int closestY = max(y1, min(yCenter, y2));

        // Distance between circle center and closest point
        long long dx = xCenter - closestX;
        long long dy = yCenter - closestY;

        // Check if the point lies inside/on the circle
        return dx * dx + dy * dy <= 1LL * radius * radius;
    }
};
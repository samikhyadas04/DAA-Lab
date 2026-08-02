#include <stdio.h>

#define MAX 100

int main()
{
    int n, a[MAX];
    int ans = -1;
    printf("Enter number of elements: ");
    scanf("%d", &n);
    if (n <= 0 || n > MAX)
    {
        printf("Invalid size. Must be between 1 and %d.\n", MAX);
        return 1;
    }
    printf("Enter elements (0s followed by 1s):\n");
    for (int i = 0; i < n; i++)
        scanf("%d", &a[i]);

    int low = 0;
    int high = n - 1;

    while (low <= high)
    {
        int mid = low + (high - low) / 2;

        if (a[mid] == 0)
        {
            low = mid + 1;
        }
        else
        {
            ans = mid;
            high = mid - 1;
        }
    }

    if (ans != -1)
        printf("Partition Point = %d\n", ans);
    else
        printf("No partition found (array is all 0s).\n");
    return 0;
}
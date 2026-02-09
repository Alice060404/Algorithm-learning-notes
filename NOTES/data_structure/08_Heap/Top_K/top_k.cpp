#include <iostream>
#include <queue>
#include <vector>
#include <stdexcept>

using namespace std;

static priority_queue<int, vector<int>, greater<int>> topKHeap(const vector<int> &nums, int k)
{
    if (k > nums.size())
        throw out_of_range("k²»ºÏ·¨");
    priority_queue<int, vector<int>, greater<int>> heap;
    for (int i = 0; i < k; i++)
    {
        heap.push(nums[i]);
    }
    for (int j = k; j < nums.size(); j++)
    {
        if (nums[j] > heap.top())
        {
            heap.pop();
            heap.push(nums[j]);
        }
    }
    return heap;
}

int main()
{
    vector<int> nums = {1, 0, 6, 4, 10, 7, 3, 8};
    priority_queue<int, vector<int>, greater<int>> heap = topKHeap(nums, 4);

    int size = heap.size();
    for (int i = 0; i < size; i++)
    {
        cout << heap.top() << " ";
        heap.pop();
    }

    return 0;
}
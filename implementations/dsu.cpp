#pragma once

#include <vector>
using namespace std;

/*
 * Disjoint Set Union (Union-Find)
 *
 * Features:
 * - Path Compression (find)
 * - Union by Rank
 * - Tracks number of connected components
 *
 * Complexity:
 * - find(): Amortized O(α(n))
 * - unite(): Amortized O(α(n))
 */

class DSU
{
private:
    vector<int> parent;
    vector<int> rankv;
    int components;

public:
    // Constructor
    explicit DSU(int n)
    {
        parent.resize(n);
        rankv.resize(n, 0);
        components = n;

        for (int i = 0; i < n; i++)
        {
            parent[i] = i;
        }
    }

    // Find with path compression
    int find(int x)
    {
        if (parent[x] != x)
        {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    // Union by rank
    bool unite(int a, int b)
    {
        int pa = find(a);
        int pb = find(b);

        if (pa == pb)
            return false;

        if (rankv[pa] < rankv[pb])
        {
            parent[pa] = pb;
        }
        else if (rankv[pb] < rankv[pa])
        {
            parent[pb] = pa;
        }
        else
        {
            parent[pb] = pa;
            rankv[pa]++;
        }

        components--;
        return true;
    }

    // Check if two nodes are connected
    bool connected(int a, int b)
    {
        return find(a) == find(b);
    }

    // Get number of components
    int count() const
    {
        return components;
    }
};
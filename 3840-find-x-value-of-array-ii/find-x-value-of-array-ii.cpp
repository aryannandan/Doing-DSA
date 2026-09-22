class Solution {
    struct Node {
        int prod;
        int cnt[5];

        Node() {
            prod = 1;
            for (int i = 0; i < 5; i++) {
                cnt[i] = 0;
            }
        }
    };

    int k;

    Node merge(const Node& A, const Node& B) {
        Node C;

        // Product of the whole segment
        C.prod = (long long)A.prod * B.prod % k;

        // Prefixes completely inside A
        for (int r = 0; r < k; r++) {
            C.cnt[r] = A.cnt[r];
        }

        // Prefixes that cross from A into B
        for (int r = 0; r < k; r++) {
            int newRem = (long long)A.prod * r % k;
            C.cnt[newRem] += B.cnt[r];
        }

        return C;
    }

public:
    vector<int> resultArray(
        vector<int>& nums,
        int k,
        vector<vector<int>>& queries
    ) {
        this->k = k;

        int n = nums.size();

        // Iterative segment tree
        int size = 1;
        while (size < n)
            size <<= 1;

        vector<Node> tree(2 * size);

        // Build leaves
        for (int i = 0; i < n; i++) {
            int rem = nums[i] % k;

            tree[size + i].prod = rem;
            tree[size + i].cnt[rem] = 1;
        }

        // Build tree
        for (int i = size - 1; i >= 1; i--) {
            tree[i] = merge(tree[i << 1], tree[i << 1 | 1]);
        }

        // Point update
        auto update = [&](int pos, int value) {
            int p = size + pos;

            int rem = value % k;

            tree[p] = Node();
            tree[p].prod = rem;
            tree[p].cnt[rem] = 1;

            p >>= 1;

            while (p >= 1) {
                tree[p] = merge(tree[p << 1], tree[p << 1 | 1]);
                p >>= 1;
            }
        };

        // Range query [l, r)
        auto query = [&](int l, int r) {
            Node leftRes;
            Node rightRes;

            l += size;
            r += size;

            while (l < r) {

                if (l & 1) {
                    leftRes = merge(leftRes, tree[l]);
                    l++;
                }

                if (r & 1) {
                    --r;
                    rightRes = merge(tree[r], rightRes);
                }

                l >>= 1;
                r >>= 1;
            }

            return merge(leftRes, rightRes);
        };

        vector<int> ans;
        ans.reserve(queries.size());

        for (auto& q : queries) {
            int index = q[0];
            int value = q[1];
            int start = q[2];
            int x = q[3];

            // Persistent update
            update(index, value);

            // Query [start, n)
            Node res = query(start, n);

            ans.push_back(res.cnt[x]);
        }

        return ans;
    }
};
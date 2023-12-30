#include <iostream>
#include <random>
#include <chrono>
#include <queue>
#include <cstring>

using namespace std;

mt19937 rng(chrono::high_resolution_clock::now().time_since_epoch().count());

void opener()
{

    cerr << " _   _          __   __                          " << endl;
    cerr << "| | | |        / _| / _|                         " << endl;
    cerr << "| |_| | _   _ | |_ | |_  _ __ ___    __ _  _ __  " << endl;
    cerr << "|  _  || | | ||  _||  _|| '_ ` _ \\  / _` || '_ \\ " << endl;
    cerr << "| | | || |_| || |  | |  | | | | | || (_| || | | |" << endl;
    cerr << "\\_| |_/ \\__,_||_|  |_|  |_| |_| |_| \\__,_||_| |_|" << endl;
    cerr << "c o d i n g " << endl;

}

FILE *fI, *fO;

struct Tree {

    Tree *left;
    Tree *right;
    unsigned char symb;
    int freq;

    class Compare{

    public:

        bool operator ()(Tree *left, Tree *right) {

            return left->freq > right->freq;
        }

    };

};

Tree *Construct(unsigned char symb, int freq, Tree *left = nullptr, Tree *right = nullptr) {

    Tree *node = new Tree();

    node->symb = symb;
    node->freq = freq;
    node->left = left;
    node->right = right;

    return node;
}

string codes[256];

void Init(Tree *root, string result) {
    if (root == nullptr) {
        return;
    }

    if (root->left == nullptr && root->right == nullptr) {
        codes[root->symb] = result;
        return;
    }

    Init (root->left, result + "0");
    Init (root->right, result + "1");

}

Tree *build(int *freq) {

    priority_queue <Tree *, vector <Tree *>, Tree::Compare> MinHeap;

    for (int i = 0; i < 256; i++) {

        if (freq[i] != 0) {
            MinHeap.push(Construct((unsigned char)i, freq[i]));
        }
    }

    while (MinHeap.size() != 1) {

        Tree *left = MinHeap.top();
        MinHeap.pop();

        Tree *right = MinHeap.top();
        MinHeap.pop();

        MinHeap.push(Construct('\0', (left->freq + right->freq), left, right));

    }

    Init(MinHeap.top(), "");

    return MinHeap.top();

}

bool bit(unsigned char ch, int i) {
    return ((ch >> i) & 1);
}

void Encode() {

    int freq[256], cnt = 0;
    unsigned char ch;

    memset(freq, 0, sizeof(freq));

    while (fscanf(fI, "%c", &ch) != -1) {
        freq[ch]++;
    }

    for (int i = 0; i < 256; i++) {
        if (freq[i]) cnt++;
    }

    fprintf(fO, "%d ", cnt);
    if (cnt == 1) {
        for (int i = 0; i < 256; i++) {
            if (freq[i]) {
                fprintf(fO, "%d ", i);
                fprintf(fO, "%d ", freq[i]);
                return;
            }
        }
    } else if (cnt < 128) {

        for (int i = 0; i < 256; i++) {

            if (freq[i]) {
                fprintf(fO, "%d ", i);
                fprintf(fO, "%d ", freq[i]);
            }
        }

    } else {

        for (int i = 0; i < 256; i++) {

            fprintf(fO, "%d ", freq[i]);
        }
    }

    rewind(fI);

    build(freq);

    unsigned char byte = 0;
    int cnt_bit = 0;

    while (fscanf(fI, "%c", &ch) != -1) {

        for (int i = 0; i < codes[ch].size(); i++) {

            byte <<= 1;
            byte += (codes[ch][i] - '0');

            cnt_bit++;

            if (cnt_bit == 8) {

                fprintf(fO, "%c", byte);

                byte = 0;
                cnt_bit = 0;
            }
        }
    }

    if (cnt_bit) {

        byte <<= (8 - cnt_bit);

        fprintf(fO, "%c", byte);
    }

}

void Decode() {

    int freq[256];
    memset(freq, 0, sizeof(freq));

    int cnt = 0, last_bit = 0, cnt_bytes = 0, cnt_last = 0, num = 0;
    unsigned char space, chr;

    fscanf(fI, "%d", &num);
    fscanf(fI, "%c", &space);

    if (num == 1) {
        int ch = 0, frq = 0;

        fscanf(fI, "%d", &ch);
        fscanf(fI, "%c", &space);

        fscanf(fI, "%d", &frq);
        fscanf(fI, "%c", &space);

        for (int i = 0; i < frq; i++) {
            fprintf(fO, "%c", (unsigned char)ch);
        }
        return;

    } else if (num < 128) {

        for (int i = 0; i < num; i++) {

            int ch = 0, frq = 0;

            fscanf(fI, "%d", &ch);
            fscanf(fI, "%c", &space);

            fscanf(fI, "%d", &frq);
            fscanf(fI, "%c", &space);

            freq[ch] = frq;
        }

    } else {

        for (int i = 0; i < 256; i++) {

            fscanf(fI, "%d", &cnt);
            fscanf(fI, "%c", &space);

            freq[i] = cnt;

        }
    }

    Tree *root = build(freq);

    Tree *node = root;

    for (int i = 0; i < 256; i++) {

        cnt_last += freq[i] * codes[i].size();

        cnt_bytes += cnt_last / 8;

        cnt_last %= 8;
    }

    cnt = 0;

    while (fscanf(fI, "%c", &chr) != -1) {

        if (cnt == cnt_bytes) {

            last_bit = 8 - cnt_last;
        }

        for (int i = 7; i >= last_bit; i--) {

            if (bit(chr, i)) {

                node = node->right;
            } else {

                node = node->left;
            }

            if (node->left == nullptr || node->right == nullptr) {

                fprintf(fO, "%c", node->symb);
                node = root;
            }
        }

        cnt++;
    }

}

int32_t main(int argc, char *argv[]) {

    chrono::_V2::system_clock::time_point start_time = chrono::high_resolution_clock::now();

    opener();

    if (!strcmp("encode", argv[1])) {

        fI = fopen(argv[3], "rb");
        fO = fopen(argv[2], "wb");

        Encode();

    } else {

        fI = fopen(argv[2], "rb");
        fO = fopen(argv[3], "wb");

        Decode();

    }

    fclose(fI);
    fclose(fO);

    chrono::_V2::system_clock::time_point end_time = chrono::high_resolution_clock::now();
    cerr << endl << "Completed Successfully" << endl << endl << (strcmp("encode", argv[1]) ? "Decompression time: " : "Compression time: ") << chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count() << " ms" << endl;

    return 0;
}

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <unordered_set>

// 计算pair的哈希值
struct pair_hash
{
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2> &p) const
    {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ h2;
    }
};

// DFA class
class DFA
{
public:
    // DFA 的状态集
    enum class State
    {
        st,
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z
    };
    
    // 终点集合
    std::unordered_set<DFA::State> end_State_with_back = {DFA::State::B, DFA::State::D, DFA::State::G, DFA::State::I, DFA::State::U};
    std::unordered_set<DFA::State> end_State_without_back = {DFA::State::J, DFA::State::K, DFA::State::L, DFA::State::M, DFA::State::N, DFA::State::O, DFA::State::P, DFA::State::Q, DFA::State::S};
    
    // 关键字集合
    std::unordered_set<std::string> key_words = {"void", "main", "int", "cout", "return"};
    std::unordered_map<std::string, int> chart = {{"void", 1}, {"main", 2}, {"int", 3}, {"cout", 4}, {"return", 5}, {"(", 6}, {")", 7}, {"{", 8}, {"}", 9}, {";", 10}, {"=", 11}, {"+", 12}, {"*", 13}, {"<<", 14}};
    
    // 状态转移图
    std::unordered_map<std::pair<DFA::State, char>, DFA::State, pair_hash> transitionTable;
    DFA();
    
    // 辅助函数
    bool find_end_state(DFA::State state);
    bool find_star_end_state(DFA::State state);
    bool find_key_words(std::string str);
};


// 构造函数初始化状态转移图
DFA::DFA()
{
    // st -> A
    this->transitionTable[{DFA::State::st, '_'}] = DFA::State::A;   // 识别下划线
    for (char c = 'a'; c <= 'z'; c++)
        this->transitionTable[{DFA::State::st, c}] = DFA::State::A; // 识别小写字母
    for (char c = 'A'; c <= 'Z'; c++)
        this->transitionTable[{DFA::State::st, c}] = DFA::State::A; // 识别大写字母

    // A -> A
    this->transitionTable[{DFA::State::A, '_'}] = DFA::State::A;    // 识别下划线
    for (char c = 'a'; c <= 'z'; c++)
        this->transitionTable[{DFA::State::A, c}] = DFA::State::A;  // 识别小写字母
    for (char c = 'A'; c <= 'Z'; c++)
        this->transitionTable[{DFA::State::A, c}] = DFA::State::A;  // 识别大写字母
    for (char c = '0'; c <= '9'; c++)
        this->transitionTable[{DFA::State::A, c}] = DFA::State::A;  // 识别数字

    // A -> B
    for (int c = CHAR_MIN; c <= CHAR_MAX; c++)                      
    {
        char ch = static_cast<char>(c);
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '_')
            continue;
        this->transitionTable[{DFA::State::A, ch}] = DFA::State::B; // 识别非字母、数字或下划线字符
    }

    // st -> C
    for (char c = '1'; c <= '9'; c++)
        this->transitionTable[{DFA::State::st, c}] = DFA::State::C; // 识别非0数字

    // C -> C
    for (char c = '0'; c <= '9'; c++)
        this->transitionTable[{DFA::State::C, c}] = DFA::State::C;  // 识别数字

    // C -> D
    for (int c = CHAR_MIN; c <= CHAR_MAX; c++)
    {
        char ch = static_cast<char>(c);
        if ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
            continue;
        this->transitionTable[{DFA::State::C, ch}] = DFA::State::D; // 识别非字母、数字字符
    }

    // C -> T
    this->transitionTable[{DFA::State::C, '_'}] = DFA::State::T;    // 识别下划线
    for (char c = 'a'; c <= 'z'; c++)
        this->transitionTable[{DFA::State::C, c}] = DFA::State::T;  // 识别小写字母
    for (char c = 'A'; c <= 'Z'; c++)
        this->transitionTable[{DFA::State::C, c}] = DFA::State::T;  // 识别大写字母

    // T -> T
    this->transitionTable[{DFA::State::T, '_'}] = DFA::State::T;    // 识别下划线
    for (char c = 'a'; c <= 'z'; c++)
        this->transitionTable[{DFA::State::T, c}] = DFA::State::T;  // 识别小写字母
    for (char c = 'A'; c <= 'Z'; c++)
        this->transitionTable[{DFA::State::T, c}] = DFA::State::T;  // 识别大写字母
    for (char c = '0'; c <= '9'; c++)
        this->transitionTable[{DFA::State::T, c}] = DFA::State::T;  // 识别数字

    // T -> U
    for (int c = CHAR_MIN; c <= CHAR_MAX; c++)
    {
        char ch = static_cast<char>(c);
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '_')
            continue;
      this->transitionTable[{DFA::State::T, ch}] = DFA::State::U;   // 识别非字母、数字或下划线字符
    }
    // st -> E
    this->transitionTable[{DFA::State::st, '0'}] = DFA::State::E;   // 识别 '0'

    // E -> D
    for (int c = CHAR_MIN; c <= CHAR_MAX; c++)
    {
        char ch = static_cast<char>(c);
        if ((ch >= '0' && ch <= '9') || ch == 'x' || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
            continue;
        this->transitionTable[{DFA::State::E, ch}] = DFA::State::D; // 识别非字母、数字
    }

    // E -> F
    for (char c = '0'; c <= '9'; c++)
        this->transitionTable[{DFA::State::E, c}] = DFA::State::F;  // 识别数字

    // F -> F
    for (char c = '0'; c <= '9'; c++)
        this->transitionTable[{DFA::State::F, c}] = DFA::State::F;  // 识别数字

    // F -> G
    for (int c = CHAR_MIN; c <= CHAR_MAX; c++)
    {
        char ch = static_cast<char>(c);
        if ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
            continue;
        this->transitionTable[{DFA::State::F, ch}] = DFA::State::G; // 识别非字母、数字
    }

    // F -> W
    this->transitionTable[{DFA::State::F, '_'}] = DFA::State::W;    // 识别下划线
    for (char c = 'a'; c <= 'z'; c++)
        this->transitionTable[{DFA::State::F, c}] = DFA::State::W;  // 识别小写字母
    for (char c = 'A'; c <= 'Z'; c++)
        this->transitionTable[{DFA::State::F, c}] = DFA::State::W;  // 识别大写字母

    // W -> W
    this->transitionTable[{DFA::State::W, '_'}] = DFA::State::W;    // 识别下划线
    for (char c = 'a'; c <= 'z'; c++)
        this->transitionTable[{DFA::State::W, c}] = DFA::State::W;  // 识别小写字母
    for (char c = 'A'; c <= 'Z'; c++)
        this->transitionTable[{DFA::State::W, c}] = DFA::State::W;  // 识别大些字母
    for (char c = '0'; c <= '9'; c++)
        this->transitionTable[{DFA::State::W, c}] = DFA::State::W;  // 识别数字

    // W -> U
    for (int c = CHAR_MIN; c <= CHAR_MAX; c++)
    {
        char ch = static_cast<char>(c);
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '_')
            continue;
        this->transitionTable[{DFA::State::W, ch}] = DFA::State::U; // 识别非字母、数字或下划线
    }

    // E -> H
    this->transitionTable[{DFA::State::E, 'x'}] = DFA::State::H;    // 识别 'x'

    // H -> H
    for (char c = '0'; c <= '9'; c++)
        this->transitionTable[{DFA::State::H, c}] = DFA::State::H;  // 识别数字
    for (char c = 'a'; c <= 'f'; c++)
        this->transitionTable[{DFA::State::H, c}] = DFA::State::H;  // 识别十六进制的字母a-f
    for (char c = 'A'; c <= 'F'; c++)
        this->transitionTable[{DFA::State::H, c}] = DFA::State::H;  // 识别十六进制的字母A-F

    // H -> I
    for (int c = CHAR_MIN; c <= CHAR_MAX; c++)
    {
        char ch = static_cast<char>(c);
        if ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
            continue;
        this->transitionTable[{DFA::State::H, ch}] = DFA::State::I; // 识别非字母、数字
    }

    // H -> X
    this->transitionTable[{DFA::State::H, '_'}] = DFA::State::X;    // 识别下划线
    for (char c = 'f'; c <= 'z'; c++)
        this->transitionTable[{DFA::State::H, c}] = DFA::State::X;  // 识别f-z字母
    for (char c = 'F'; c <= 'Z'; c++)
        this->transitionTable[{DFA::State::H, c}] = DFA::State::X;  // 识别F-Z字母

    // X -> X
    this->transitionTable[{DFA::State::X, '_'}] = DFA::State::X;    // 识别下划线
    for (char c = 'a'; c <= 'z'; c++)
        this->transitionTable[{DFA::State::X, c}] = DFA::State::X;  // 识别小写字母
    for (char c = 'A'; c <= 'Z'; c++)
        this->transitionTable[{DFA::State::X, c}] = DFA::State::X;  // 识别大写字母
    for (char c = '0'; c <= '9'; c++)
        this->transitionTable[{DFA::State::X, c}] = DFA::State::X;  // 识别数字

    // X -> U
    for (int c = CHAR_MIN; c <= CHAR_MAX; c++)
    {
        char ch = static_cast<char>(c);
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '_')
            continue;
        this->transitionTable[{DFA::State::X, ch}] = DFA::State::U; // 识别非字母、数字或下划线
    }

    // E -> V
    for (char c = 'a'; c <= 'z'; c++)
        if (c != 'x')
            this->transitionTable[{DFA::State::E, c}] = DFA::State::V;  // 识别非x
    for (char c = 'A'; c <= 'Z'; c++)
        if (c != 'X')
            this->transitionTable[{DFA::State::E, c}] = DFA::State::V;  // 识别非x

    // V -> V
    this->transitionTable[{DFA::State::V, '_'}] = DFA::State::V;        // 识别下划线
    for (char c = 'a'; c <= 'z'; c++)
        this->transitionTable[{DFA::State::V, c}] = DFA::State::V;      // 识别小写字母
    for (char c = 'A'; c <= 'Z'; c++)
        this->transitionTable[{DFA::State::V, c}] = DFA::State::V;      // 识别大写字母
    for (char c = '0'; c <= '9'; c++)
        this->transitionTable[{DFA::State::V, c}] = DFA::State::V;      // 识别数字

    // V -> U
    for (int c = CHAR_MIN; c <= CHAR_MAX; c++)
    {
        char ch = static_cast<char>(c);
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '_')
            continue;
        this->transitionTable[{DFA::State::V, ch}] = DFA::State::U;     // 识别非字母、数字或下划线
    }

    // st -> J
    this->transitionTable[{DFA::State::st, '('}] = DFA::State::J; // 识别 '('
    // st -> K
    this->transitionTable[{DFA::State::st, ')'}] = DFA::State::K; // 识别 ')'
    // st -> L
    this->transitionTable[{DFA::State::st, '{'}] = DFA::State::L; // 识别 '{'
    // st -> M
    this->transitionTable[{DFA::State::st, '}'}] = DFA::State::M; // 识别 '}'
    // st -> N
    this->transitionTable[{DFA::State::st, ';'}] = DFA::State::N; // 识别 ';'
    // st -> O
    this->transitionTable[{DFA::State::st, '='}] = DFA::State::O; // 识别 '='
    // st -> P
    this->transitionTable[{DFA::State::st, '+'}] = DFA::State::P; // 识别 '+'
    // st -> Q
    this->transitionTable[{DFA::State::st, '*'}] = DFA::State::Q; // 识别 '*'
    // st -> R
    this->transitionTable[{DFA::State::st, '<'}] = DFA::State::R; // 识别 '<'
    // R -> S
    this->transitionTable[{DFA::State::R, '<'}] = DFA::State::S;  // 识别 '<'
}


// degug 辅助函数
std::ostream &operator<<(std::ostream &os, const DFA::State &state)
{
    switch (state)
    {
    case DFA::State::st:
        os << "st";
        break;
    case DFA::State::A:
        os << "A";
        break;
    case DFA::State::B:
        os << "B";
        break;
    case DFA::State::C:
        os << "C";
        break;
    case DFA::State::D:
        os << "D";
        break;
    case DFA::State::E:
        os << "E";
        break;
    case DFA::State::F:
        os << 'F';
        break;
    case DFA::State::G:
        os << 'G';
        break;
    case DFA::State::H:
        os << 'H';
        break;
    case DFA::State::I:
        os << 'I';
        break;
    case DFA::State::J:
        os << 'J';
        break;
    case DFA::State::K:
        os << 'K';
        break;
    case DFA::State::L:
        os << 'L';
        break;
    case DFA::State::M:
        os << 'M';
        break;
    case DFA::State::N:
        os << 'N';
        break;
    case DFA::State::O:
        os << 'O';
        break;
    case DFA::State::P:
        os << 'P';
        break;
    case DFA::State::Q:
        os << 'Q';
        break;
    case DFA::State::R:
        os << 'R';
        break;
    case DFA::State::S:
        os << 'S';
        break;
    case DFA::State::T:
        os << 'T';
        break;
    case DFA::State::U:
        os << 'U';
        break;
    case DFA::State::V:
        os << 'V';
        break;
    case DFA::State::W:
        os << 'W';
        break;
    case DFA::State::X:
        os << 'X';
        break;

    default:
        break;
        // ...
    }
    return os;
}

// 读取源文件内容，返回字符串
std::string read_file(std::string path)
{
    // open file
    std::ifstream file(path);

    // convert to string
    std::string str((std::istreambuf_iterator<char>(file)),
                    std::istreambuf_iterator<char>());
    return str;
}

// 处理文件内容，写入到新文件
void pre_process(std::string &file)
{
    for (int i = 0; i < file.size(); i++)
    {
        // remove "//" comment
        if (file[i] == '/' && file[i + 1] == '/')
        {
            while (file[i] != '\n')
            {
                file.erase(i, 1);
            }
        }
        //  remove "/*...*/ comment"
        if (file[i] == '/' && file[i + 1] == '*')
        {
            while (file[i] != '*' || file[i + 1] != '/')
            {
                file.erase(i, 1);
            }
            // remove "*/"
            file.erase(i, 1);
            file.erase(i, 1);
        }
    }

    // remove '\n' '\t' character
    for (int i = 0; i < file.size(); i++)
    {
        if (file[i] == '\n')
            file.erase(i, 1);
        if (file[i] == '\t')
            file.erase(i, 1);
    }

    // wirte new file
    std::ofstream f("pre_processed_text.txt");
    f << file;
    f.close();
}

// 寻找带*终态
bool DFA::find_star_end_state(DFA::State state)
{
    return this->end_State_with_back.find(state) != this->end_State_with_back.end();
}

// 寻找非带*终态
bool DFA::find_end_state(DFA::State state)
{
    return this->end_State_without_back.find(state) != this->end_State_without_back.end();
}

// 查询关键字
bool DFA::find_key_words(std::string str)
{
    return this->key_words.find(str) != this->key_words.end();
}

void run()
{
    // 读文件并预处理
    std::string file = read_file("source.txt");
    pre_process(file);

    // 读预处理的文件
    file = read_file("pre_processed_text.txt");

    DFA dfa;
    // 初始化状态
    DFA::State state = DFA::State::st;
    // 初始化非法数组
    std::vector<std::pair<std::string, int>> illegal;
    std::string token = "";
    int count = 1;
    
    std::ofstream res_file("res.txt");
    
    res_file << "\t\t单词序号\t单词类型\t单词的值\n";
    
      
    // 使用DFA读入字符
    for (int i = 0; i < file.size(); i++)
    {
        if (file[i] == ' ')
            continue;
        // 到终态后停止
        while (dfa.transitionTable.count({state, file[i]}) > 0)
        {
            state = dfa.transitionTable[{state, file[i]}];
            if (!dfa.find_star_end_state(state) || dfa.find_end_state(state))
                token += file[i];
            i++;
        }
      
        // 如果是终态，则打印
        if (dfa.find_star_end_state(state) || dfa.find_end_state(state))
        {
            switch (state)
            {
            case DFA::State::B: // 关键字和标识符
                if (dfa.find_key_words(token))
                    res_file << "\t\t" << count << "\t\t" << dfa.chart[token] << "\t\t" << token << std::endl;
                else
                    res_file << "\t\t" << count << "\t\t"
                              << "15"
                              << "\t\t" << token << std::endl;
                break;
            case DFA::State::D: // 十进制数
                res_file << "\t\t" << count << "\t\t"
                          << "16"
                          << "\t\t" << token << std::endl;
                break;
            case DFA::State::G: // 八进制数
                res_file << "\t\t" << count << "\t\t"
                          << "17"
                          << "\t\t" << token << std::endl;
                break;
            case DFA::State::I: // 十六进制数
                res_file << "\t\t" << count << "\t\t"
                          << "18"
                          << "\t\t" << token << std::endl;
                break;
            case DFA::State::U: // 非法标识符
                illegal.push_back({token, count});
                break;
            case DFA::State::J: // (
                res_file << "\t\t" << count << "\t\t" << dfa.chart[token] << "\t\t" << token << std::endl;
                i++;
                break;
            case DFA::State::K: // )
                res_file << "\t\t" << count << "\t\t" << dfa.chart[token] << "\t\t" << token << std::endl;
                i++;
                break;
            case DFA::State::L: // {
                res_file << "\t\t" << count << "\t\t" << dfa.chart[token] << "\t\t" << token << std::endl;
                i++;
                break;
            case DFA::State::M: // }
                res_file << "\t\t" << count << "\t\t" << dfa.chart[token] << "\t\t" << token << std::endl;
                i++;
                break;
            case DFA::State::N: // ;
                res_file << "\t\t" << count << "\t\t" << dfa.chart[token] << "\t\t" << token << std::endl;
                i++;
                break;
            case DFA::State::O: // =
                res_file << "\t\t" << count << "\t\t" << dfa.chart[token] << "\t\t" << token << std::endl;
                i++;
                break;
            case DFA::State::P: // +
                res_file << "\t\t" << count << "\t\t" << dfa.chart[token] << "\t\t" << token << std::endl;
                i++;
                break;
            case DFA::State::Q: // *
                res_file << "\t\t" << count << "\t\t" << dfa.chart[token] << "\t\t" << token << std::endl;
                i++;
                break;
            case DFA::State::S: // <<
                res_file << "\t\t" << count << "\t\t" << dfa.chart[token] << "\t\t" << token << std::endl;
                i++;
                break;
            default:
                // 非法标识符
                illegal.push_back({token, count});
                break;
            }
          // 回到初态
            state = DFA::State::st;
            token = "";
            i -= 2;
            count++;
        }
    }
    
    // 在源文件中查询位置
    file = read_file("source.txt");
    // 遍历非法标识符
    for (auto item : illegal)
    {
        int index = file.find(item.first);
        
        // 计算位置
        int row = 0;
        int colmn = 0;
        for (int i = 0; i < index; i++)
        {
            colmn++;
            if (file[i] == '\n')
            {
                row++;
                colmn = 0;
            }
        }
        res_file << "发现非法标识符: " << item.first << " 位于源程序第" << item.second << "个单词 "
                  << "第" << row + 1 << "行,"
                  << "第" << colmn + 1 << "列\n";
    }
    return;
}

int main()
{
    run();
}
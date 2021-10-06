#include <cassert>
#include <cstddef>
#include <cstdint>
#include <string>
#include <array>
#include <algorithm>

using namespace std;

const std::array<char, 4> CODE = { 'A', 'T', 'G', 'C' };

struct Nucleotide {
    char symbol;
    size_t position;
    int chromosome_num;
    int gene_num;
    bool is_marked;
    char service_info;
};

struct CompactNucleotide {
    uint8_t service_info;   //          8 bit
    
    uint8_t chromosome_num : 6; //1..46     6 bit
    uint8_t symbol : 2;         //0..4      2 bit
    
    uint16_t gene_num : 15;     //0..25k    15 bit
    uint16_t is_marked : 1;      //          1 bit
    uint32_t position : 32;     //0..3kkk   32 bit
};

CompactNucleotide Compress(const Nucleotide& n) {
    CompactNucleotide cd;
    cd.is_marked = n.is_marked;
    
    auto it = std::find(CODE.begin(), CODE.end(), n.symbol);
    cd.symbol = std::distance(CODE.begin(), it);
    auto x = n.service_info;
    cd.service_info = x;
    cd.position = n.position;
    cd.chromosome_num = n.chromosome_num;
    cd.gene_num = n.gene_num;
    
    return cd;
}

Nucleotide Decompress(const CompactNucleotide& cn) {
    Nucleotide nc;
    nc.symbol = CODE[cn.symbol];
    nc.position = cn.position;
    nc.chromosome_num = cn.chromosome_num;
    nc.gene_num = cn.gene_num;
    nc.is_marked = cn.is_marked;
    nc.service_info = cn.service_info;
    return nc;
}

//static_assert(sizeof(CompactNucleotide) <= 8, "Your CompactNucleotide is not compact enough"s);
//static_assert(alignof(CompactNucleotide) == 4, "Don't use '#pragma pack'!"s);
bool operator==(const Nucleotide& lhs, const Nucleotide& rhs) {
    return (lhs.symbol == rhs.symbol) && (lhs.position == rhs.position) && (lhs.chromosome_num == rhs.chromosome_num)
        && (lhs.gene_num == rhs.gene_num) && (lhs.is_marked == rhs.is_marked) && (lhs.service_info == rhs.service_info);
}
void TestSize() {
    assert(sizeof(CompactNucleotide) <= 8);
}
void TestCompressDecompress() {
    Nucleotide source;
    source.symbol = 'T';
    source.position = 1'000'000'000;
    source.chromosome_num = 48;
    source.gene_num = 1'000;
    source.is_marked = true;
    source.service_info = '!';

    CompactNucleotide compressedSource = Compress(source);
    Nucleotide decompressedSource = Decompress(compressedSource);


    assert(source == decompressedSource);    

    Nucleotide s1;
    s1.symbol = 'A';
    s1.position = 0;
    s1.chromosome_num = 0;
    s1.gene_num = 0;
    s1.is_marked = false;
    s1.service_info = '0';

    Nucleotide r1 = Decompress(Compress(s1));

    assert(s1 == r1);

    Nucleotide s2;
    s2.symbol = 'T';
    s2.position = 564;
    s2.chromosome_num = 50;
    s2.gene_num = 5615;
    s2.is_marked = false;
    s2.service_info = char(255);

    Nucleotide r2 = Decompress(Compress(s2));

    assert(s2 == r2);

    Nucleotide s3;
    s3.symbol = 'A';
    s3.position = 0;
    s3.chromosome_num = 0;
    s3.gene_num = 0;
    s3.is_marked = false;
    s3.service_info = char(0);

    Nucleotide r3 = Decompress(Compress(s3));

    assert(s3 == r3);

    Nucleotide s4;
    s4.symbol = 'C';
    s4.position = 3300000000;
    s4.chromosome_num = 46;
    s4.gene_num = 25000;
    s4.is_marked = false;
    s4.service_info = '`';

    Nucleotide r4 = Decompress(Compress(s4));

    assert(s4 == r4);
}

int main() {
    TestSize();
    TestCompressDecompress();
}
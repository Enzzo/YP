#include <iostream>
#include <vector>
#include <algorithm>

struct Document {
	int id;
	int rating;
};

void PrintDocuments(std::vector<Document> documents, size_t skip_start, size_t skip_finish) {

    if (documents.size() > (skip_start + skip_finish)) {

        std::sort(documents.begin(), documents.end(), [](const Document& lhd, const Document& rhd) {
            return lhd.rating > rhd.rating;
            });

        for (size_t i = skip_start; i < documents.size() - skip_finish; ++i)
            std::cout << "{ id = " << documents[i].id << ", rating = " << documents[i].rating<<" }" << std::endl;
    }
}

int main() {
    PrintDocuments(
        {
            {100, 5},
            {101, 7},
            {102, -4},
            {103, 9},
            {104, 1}
        },
        1,
        2
    );
	return 0;
}
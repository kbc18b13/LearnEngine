#pragma once
namespace LearnEngine {

class NonSkinModel;

std::unique_ptr<NonSkinModel> loadNonSkinModel(const char* filePath);

}


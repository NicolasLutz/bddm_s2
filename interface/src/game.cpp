#include "game.h"

Game::Game(std::string const& name, QImage const& analysis,
           std::string* editor, std::string* description, QImage const& img, int* year) :
    m_name(name),
    m_analysisImg(analysis),
    m_gameImg(img),
    m_editor(editor),
    m_description(description),
    m_year(year),
    m_dirty(false)
{

}

std::string const& Game::name() const {
    return m_name;
}

const std::string* Game::editor() const {
    return m_editor;
}

const std::string* Game::description() const {
    return m_description;
}

QImage const& Game::analysis() const {
    return m_analysisImg;
}

QImage const& Game::gameImg() const {
    return m_gameImg;
}

const int* Game::year() const {
    return m_year;
}

void Game::setName(const std::string& name) {
    if(name != m_name) {
        m_dirty = true;
        m_name = name;
    }
}

void Game::setAnalysis(const QImage& analysis) {
    if(analysis != m_analysisImg) {
        m_dirty = true;
        m_analysisImg = analysis;
    }
}

void Game::setGameImg(QImage& gameImg) {
    if(gameImg != m_gameImg) {
        m_dirty = true;
        m_gameImg = gameImg;
    }
}

void Game::setEditor(std::string* editor) {
    setPtrMember<std::string>(m_editor, editor);
}

void Game::setDescription(std::string* description) {
    setPtrMember<std::string>(m_description, description);
}


void Game::setYear(int *year) {
    setPtrMember<int>(m_year, year);
}

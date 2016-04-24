#include "game.h"

Game::Game(QString const& name, QImage const& analysis,
           QString* editor, QString* description, QImage const& img, int* year) :
    m_name(name),
    m_analysisImg(analysis),
    m_gameImg(img),
    m_editor(editor),
    m_description(description),
    m_year(year),
    m_dirty(false)
{

}

Game::~Game()
{
    if(m_editor)
        delete m_editor;
    if(m_year)
        delete m_year;
    if(m_description)
        delete m_editor;
}

QString const& Game::name() const {
    return m_name;
}

const QString* Game::editor() const {
    return m_editor;
}

const QString* Game::description() const {
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

void Game::setName(const QString& name) {
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

void Game::setEditor(QString* editor) {
    setPtrMember<QString>(m_editor, editor);
}

void Game::setDescription(QString* description) {
    setPtrMember<QString>(m_description, description);
}


void Game::setYear(int *year) {
    setPtrMember<int>(m_year, year);
}

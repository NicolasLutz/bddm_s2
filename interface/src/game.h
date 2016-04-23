#ifndef GAME_H
#define GAME_H

#include <string>
#include <qimage.h>

class Game
{
    public:
        Game(std::string const& name, QImage const& analysis,
             std::string* editor, std::string* description, QImage const& img, int* year);

        // Getters
        std::string const& name() const;
        const std::string* editor() const;
        const std::string* description() const;
        QImage const& analysis() const;
        QImage const& gameImg() const;
        const int* year() const;

        // Setters
        void setName(std::string const& name);
        void setEditor(std::string* editor);
        void setDescription(std::string* description);
        void setAnalysis(QImage const& analysis);
        void setGameImg(QImage& gameImg);
        void setYear(int* year);

    protected:
        std::string  m_name;
        QImage       m_analysisImg;
        QImage       m_gameImg;
        std::string* m_editor;
        std::string* m_description;
        int*         m_year;

        bool m_dirty;

    private:
        template <class T>
        void setPtrMember(T* member, T* value);
};

#include "game.inl"

#endif // GAME_H

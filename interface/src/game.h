#ifndef GAME_H
#define GAME_H

#include <qstring.h>
#include <qimage.h>

class Game
{
    public:
        Game();
        Game(QString const& name, QImage const& analysis,
             QString* editor, QString* description, QImage const& img, int* year);

        virtual ~Game();
        void operator=(Game const& other);

        // Getters
        QString const& name() const;
        const QString* editor() const;
        const QString* description() const;
        QImage const& analysis() const;
        QImage const& gameImg() const;
        const int* year() const;

        // Setters
        void setName(QString const& name);
        void setEditor(QString* editor);
        void setDescription(QString* description);
        void setAnalysis(QImage const& analysis);
        void setGameImg(QImage& gameImg);
        void setYear(int* year);

    protected:
        QString  m_name;
        QImage   m_analysisImg;
        QImage   m_gameImg;
        QString* m_editor;
        QString* m_description;
        int*     m_year;

        bool m_dirty;

    private:
        template <class T>
        void setPtrMember(T* member, T* value);
};

#include "game.inl"

#endif // GAME_H

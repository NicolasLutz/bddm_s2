template <class T>
void Game::setPtrMember(T* member, T* value) {
    if(value) {
        if(!member) {
            m_dirty = true;
            member = new T(*value);
        }
        else if(*member != *value) {
            m_dirty = true;
            delete member;
            member = new T(*value);
        }
    }
    else { // null value
        if(member) {
            m_dirty = true;
            delete member;
            member = nullptr;
        }
    }
}

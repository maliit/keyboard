#ifndef CANDIDATESCALLBACK_H
#define CANDIDATESCALLBACK_H

#include <presage.h>

class CandidatesCallback
    : public PresageCallback
{
private:
    const std::string& m_past_context;
    const std::string m_empty;

public:
    explicit CandidatesCallback(const std::string& past_context);

    std::string get_past_stream() const;
    std::string get_future_stream() const;
};

#endif // CANDIDATESCALLBACK_H

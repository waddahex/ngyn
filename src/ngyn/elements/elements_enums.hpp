#pragma once

namespace ngyn
{
  enum class Positioning
  {
    Start,
    Middle,
    End
  };

  struct Alignment : public glm::vec<2, Positioning>
  {
    Alignment() :
      glm::vec<2, Positioning>(Positioning::Start, Positioning::Start){}
  };
};
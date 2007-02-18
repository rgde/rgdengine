#pragma once

namespace input
{

  //перечисление существующих команд
  enum ECommand
  {
    CommandUp,
    CommandDown,
    CommandRotateCW,
    CommandRotateCCW,
    CommandForward,
    CommandBackward,
    CommandStarfeLeft,
    CommandStarfeRight,
    CommandSlide,
    CommandRotateOX,
    CommandRotateOY,
    CommandEscape,
    //... другие команды

    CommandEnumSize,
    InvalidCommand,
  };

  const std::string& ECommand2String (ECommand);           //получение строки, соответствующей enum-значению
  ECommand           String2ECommand (const std::string&); //получение enum-значения, соответствующего строке

}
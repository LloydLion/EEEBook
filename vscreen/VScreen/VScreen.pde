import processing.serial.*;

Serial serial;
PFont f;

void setup()
{
   size(300, 400);
   background(255);
   
   serial = new Serial(this, "COM3", 9600);
   f = createFont("Arial", 15, true);
}

void draw()
{
  while (serial.available() > 0)
  {
    delay(50);
    String drawCall = readUntil('\n');
    println(drawCall);
    
    try
    {
      
      if (drawCall.startsWith("rect: "))
      {
        String[] data = split(drawCall.substring(6), ',');
        int start_x = Integer.parseInt(data[0]);
        int start_y = Integer.parseInt(data[1]);
        int rect_width = Integer.parseInt(data[2]);
        int rect_height = Integer.parseInt(data[3]);
        
        stroke(0);
        fill(50);
        rect(start_x, start_y, rect_width, rect_height);
      }
      else if (drawCall.startsWith("text: "))
      {
        String[] data = split(drawCall.substring(6), ',');
        int start_x = Integer.parseInt(data[0]);
        int start_y = Integer.parseInt(data[1]);
        
        String text = readUntil('\0'); 
        serial.read();
        
        println(text);
        
        textAlign(LEFT);
        textFont(f);
        fill(0);
        text(text, start_x, start_y + 15);
      }
    }
    catch (Exception ex)
    {
      println(ex);
    }
  }
}

String readUntil(char terminator)
{
  String result = "";
  while (true)
  {
    char read_res = serial.readChar();
    if (read_res == -1 || read_res == 0xffff)
      continue;
    if (read_res == terminator)
      return result;
    
    result += read_res;
  }
}

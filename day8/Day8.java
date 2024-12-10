import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

class Point {
    public int _x, _y;

    public Point(int x, int y) {
        _x = x;
        _y = y;
    }

    Point mult(int x) { return new Point(_x*x, _y*x); }

    @Override
    public String toString() {
        return "(" + _x + "," + _y + ")";
    }

    @Override
    public boolean equals(Object obj) {
        Point obj2 = (obj instanceof Point) ?((Point)obj) :null;
        return obj2 != null && obj2._x == this._x && obj2._y == this._y;

    }

    @Override
    public int hashCode() {
        return _x*100 + _y;
    }
}

class Input {
    private Map<Character, List<Point>> _antennas;
    private int _width, _height;
    private List<String> _map;

    public Input() {
        _antennas = new HashMap<>();
        _map = new ArrayList<>();
        
        parseInputFile();
    }

    private void parseInputFile() {
        File file = new File("input.txt");
        try (BufferedReader br = new BufferedReader(new FileReader(file))) {

            Pattern pattern = Pattern.compile("[a-zA-Z0-9]");

            for (String line = br.readLine() ; line != null ; line = br.readLine()) {
                if (line.length() < 1) return;
                line = line.replace("\n", "");
                _width = line.length();
                _height++;
                _map.add(line);

                Matcher matcher = pattern.matcher(line);

                matcher.results().forEach((antenna) -> {
                    char antennaChar = antenna.group().charAt(0);
                    if (!_antennas.containsKey(antennaChar)) {
                        _antennas.put(antennaChar, new ArrayList<>());
                    }
                    _antennas.get(antennaChar).add(new Point(antenna.start(), _height - 1));
                });
            }

            br.close();
        }
        catch (FileNotFoundException e) {
            System.out.println("File not found");
        } catch (IOException e) {
            System.out.println("IO Exception");
        }
    }

    public boolean inBounds (Point p) {
        return p._x >= 0 && p._x < _width && p._y >= 0 && p._y < _height;
    }

    public Map<Character, List<Point>> getAntennas() { return _antennas; }
    public int getWidth () { return _width; }
    public int getHeight () { return _height; }

    public void printAntinodes(Set<Point> antinodes) {
        for (int i=0 ; i<_height ; i++) {
            for (int j=0 ; j<_width ; j++) {
                if (antinodes.contains(new Point(j,i)))
                    System.out.print("#");
                else
                    System.out.print(_map.get(i).charAt(j));
            }
            System.out.println();
        }
    }
}

class Challenge1 {
    private final Input _input;

    public Challenge1 (Input input) {
        _input = input;
    }

    public void solve() {
        Set<Point> antinodes = new HashSet<>();
        Map<Character, List<Point>> antennas = _input.getAntennas();

        antennas.entrySet().forEach(entry -> {
            List<Point> positions = entry.getValue();

            
            positions.forEach(point1 -> {
                positions.forEach(point2 -> {
                    if (point1 != point2) {

                        Point delta = new Point(point2._x - point1._x,
                                                point2._y - point1._y);
                        
                        Point pos = new Point(point1._x - delta._x,
                                                point1._y - delta._y);

                        if (_input.inBounds(pos))
                            antinodes.add(pos);
                        
                        pos = new Point(point2._x + delta._x, point2._y + delta._y);

                        if (_input.inBounds(pos))
                            antinodes.add(pos);
                    }
                });
            });
        });

        System.out.println("Challenge1: " + antinodes.size());
    }
}


class Challenge2 {
    private final Input _input;

    public Challenge2 (Input input) {
        _input = input;
    }

    public void solve() {
        Set<Point> antinodes = new HashSet<>();
        Map<Character, List<Point>> antennas = _input.getAntennas();

        antennas.entrySet().forEach(entry -> {
            List<Point> positions = entry.getValue();

            positions.forEach(point1 -> {
                positions.forEach(point2 -> {
                    if (point1 != point2) {

                        Point deltaMin = new Point(point2._x - point1._x,
                                                    point2._y - point1._y);
                        
                        Point delta;
                        for (int i=0 ; ; i++) {
                            boolean flag1, flag2;
                            delta = deltaMin.mult(i);

                            Point pos1 = new Point(point1._x - delta._x, point1._y - delta._y);

                            flag1 = _input.inBounds(pos1);
                            if (flag1)
                                antinodes.add(pos1);
                            
                            Point pos2 = new Point(point2._x + delta._x, point2._y + delta._y);

                            flag2 = _input.inBounds(pos2);
                            if (flag2)
                                antinodes.add(pos2);
                                
                            if (!(flag1 || flag2))
                                break;
                        }
                    }
                });
            });
        });


        _input.printAntinodes(antinodes);
        System.out.println("Challenge2: " + antinodes.size());
    }
}

public class Day8 {

    
    public static void main(String[] args) {
        Input input = new Input();
        Challenge1 challenge1 = new Challenge1(input);
        Challenge2 challenge2 = new Challenge2(input);

        challenge1.solve();
        challenge2.solve();
    }
}
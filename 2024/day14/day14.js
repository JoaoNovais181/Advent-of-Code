const fs = require('fs');


const MAP_WIDTH = 101;
const MAP_HEIGHT = 103;
const HALF_WIDTH = Math.floor(MAP_WIDTH / 2);
const CORRIDOR_WIDTH = Math.ceil(MAP_WIDTH/2);
const HALF_HEIGHT = Math.floor(MAP_HEIGHT / 2);
const CORRIDOR_HEIGHT = Math.ceil(MAP_HEIGHT/2);

class Robot {
  constructor(posX, posY, velX, velY) {
    this.posX = posX;
    this.posY = posY;
    this.velX = velX;
    this.velY = velY;
  }
}

function read_input() {
  let input = [];
  const data = fs.readFileSync('input.txt', 'utf-8');
  const lines = data.split('\n');

  lines.forEach(line => {
    if (line.length < 1) return;

    const re = /^p=(\d+),(\d+) v=(-?\d+),(-?\d+)$/g;
    const vals = re.exec(line);
    input.push(new Robot(BigInt(vals[1]), BigInt(vals[2]), BigInt(vals[3]), BigInt(vals[4])));
  });
  return input;
}

function pass_N_seconds(num_seconds, robot) {
  let finalX = (robot.posX + robot.velX * BigInt(num_seconds)) % BigInt(MAP_WIDTH);
  let finalY = (robot.posY + robot.velY * BigInt(num_seconds)) % BigInt(MAP_HEIGHT);

  if (finalX < 0)
    finalX = BigInt(MAP_WIDTH) + finalX;

  if (finalY < 0)
    finalY = BigInt(MAP_HEIGHT) + finalY;

  return new Robot(finalX, finalY, robot.velX, robot.velY);
}

function get_quadrant(robot) {
  if (robot.posX < HALF_WIDTH) {
    if (robot.posY < HALF_HEIGHT)
      return 2;

    if (robot.posY < CORRIDOR_HEIGHT)
      return 0;
    return 3;
  }
  
  if (robot.posX < CORRIDOR_WIDTH)
    return 0;

  if (robot.posY < HALF_HEIGHT)
    return 1;
  if (robot.posY < CORRIDOR_HEIGHT)
    return 0;
  return 4;
}

function safety_rating(robots) {
  const quadrants = [0,0,0,0,0];
  robots.forEach(robot => {
    quadrants[get_quadrant(robot)] += 1;
  });
  return (quadrants[1]*quadrants[2]*quadrants[3]*quadrants[4]);
}

function challenge1(input) {
  const final = [];

  input.forEach(robot => {
    final.push(pass_N_seconds(100, robot));
  });

  console.log("Challenge1: " + safety_rating(final));
}

function map_to_str(robots) {
  let out = "";
  for (let i=0 ; i<MAP_HEIGHT ; i++) {
    for (let j=0 ; j<MAP_WIDTH ; j++) {
      let count = 0;
      for (let k=0 ; k<robots.length ; k++) {
        if (robots[k].posX == j && robots[k].posY == i)
          count++;
      }

      if (count == 0)
        out += '.';
      else
        out += ""+count;
    }
    out += '\n';
  }
  return out;
}

function tick(robots) {
  let final = [];
  robots.forEach(robot => {
    final.push(pass_N_seconds(1, robot));
  });
  return final;
}

function challenge2(input) {
  let robots = [];

  for (let i=1 ; i<MAP_WIDTH * MAP_HEIGHT ; i++) {
    input.forEach(robot => {
      robots.push(pass_N_seconds(1, robot));
    });
    let str = "" + i + '\n' + map_to_str(robots) + "\n";
    fs.appendFileSync("output.txt", str);
    input = robots;
    robots = [];
  }
  // let found = -1;
  // for (let i=1 ; i<10000 && found===-1 ; i++) {
  //   input = tick(input);
  //   if ((/1111111/g).test(map_to_str(input)))
  //     found = i;
  // }

  // console.log("Challenge2: " +  found);
}

function main () {
  const input = read_input();

  challenge1(input);
  challenge2(input);
}

main();

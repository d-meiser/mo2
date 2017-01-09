#include <initial_guess.h>
#include <utilities.h>
#include <cmath>
#include <iostream>


namespace Mo {

namespace {

void insertTile(Badness* badness, Mosaic* mosaic,
    float dx, float dy, const Tile& tile) {

  float x0 = -0.5 * mosaic->targetImage().width() + 0.5 * dx;
  float y0 = -0.5 * mosaic->targetImage().height() + 0.5 * dy;

  mosaic->addTiles(&tile, &tile + 1);
  auto t = mosaic->tilesEnd();
  t--;
  t->x_ = x0;
  t->y_ = y0;
  t->angle_ = 0.0f;
  t->scale_ = 1.0f;
  float newScale = std::sqrt((dx / t->width()) * (dy / t->height()));
  t->scale_ = newScale;

  float bestBadness = badness->computeBadness(*mosaic);

  float y = y0;
  float bestX = x0;
  float bestY = y0;
  while (y < mosaic->targetImage().height()) {
    float x = x0;
    while (x < mosaic->targetImage().width()) {
      t->x_ = x;
      t->y_ = y;
      float b = badness->computeBadness(*mosaic);
      std::cout << " ------ \n";
      std::cout << t->x_ << std::endl;
      std::cout << t->y_ << std::endl;
      std::cout << t->angle_ << std::endl;
      std::cout << t->scale_ << std::endl;
      std::cout << t->width() << std::endl;
      std::cout << t->height() << std::endl;
      std::cout << " b == " << b << std::endl;
      if (b < bestBadness) {
        bestBadness = b;
        bestX = x;
        bestY = y;
      }
      x += dx;
    }
    y += dy;
  }

  t->x_ = bestX;
  t->y_ = bestY;

  std::cout << " ------ Chosen Tile:" << std::endl;
  std::cout << " >>> " << t->x_ << std::endl;
  std::cout << " >>> " << t->y_ << std::endl;
  std::cout << " >>> " << t->angle_ << std::endl;
  std::cout << " >>> " << t->scale_ << std::endl;
  std::cout << " >>> " << t->width() << std::endl;
  std::cout << " >>> " << t->height() << std::endl;
  std::cout << "---------------"<< std::endl;
}

}

Mosaic InitialGuess::findInitialGuess(Badness* badness, const Mosaic& mosaic) {
  const Mo::TargetImage& targetImage = mosaic.targetImage();
  Mosaic guess{targetImage.image(), targetImage.scale()};

  float n = std::sqrt(mosaic.getTiles().size());
  float dx = mosaic.targetImage().width() / n;
  float dy = mosaic.targetImage().height() / n;
  std::cout << " n == " << n << std::endl;
  std::cout << " dx == " << dx << std::endl;
  std::cout << " dy == " << dy << std::endl;

  for (size_t i = 0; i < mosaic.getTiles().size(); ++i) {
    insertTile(badness, &guess, dx, dy, mosaic.getTiles()[i]);
  }

  return guess;
}

}


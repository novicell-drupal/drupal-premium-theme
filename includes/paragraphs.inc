<?php

use Drupal\Core\TypedData\Exception\MissingDataException;
use Drupal\Core\Url;
use Drupal\paragraphs\Entity\Paragraph;

/**
 * Change hero paragraph, add media and page title.
 *
 * @param array $variables
 *   Theme variables.
 */
function premium_theme_preprocess_paragraph__basic_hero(array &$variables) {
  /** @var Paragraph $paragraph */
  $paragraph = $variables['elements']['#paragraph'];

  if (isset($variables['elements']['#title'])) {
    $variables['title'] = $variables['elements']['#title'];
  }

  if ($paragraph->hasField('field_background') && !$paragraph->get('field_background')->isEmpty()) {
    $media = $paragraph->get('field_background')->entity;
    if (!is_null($media)) {
      $variables['media_image'] = \Drupal::entityTypeManager()->getViewBuilder('media')->view($media, 'hero');
      if (in_array($media->bundle(), ['remote_video', 'cdn_video'])) {
        $variables['media'] = \Drupal::entityTypeManager()->getViewBuilder('media')->view($media);
      }
    }
  }

  if ($paragraph->hasField('field_cta') && !$paragraph->get('field_cta')->isEmpty()) {
    try {
      $button = $paragraph->get('field_cta')->first();

      if (!is_null($button)) {
        $button = $button->getValue();

        if (isset($button['uri'], $button['title'])) {
          $variables['link']['title'] = $button['title'];
          $variables['link']['url'] = Url::fromUri($button['uri'])->toString();

          if (isset($button['target_blank']) && (int) $button['target_blank'] === 1) {
            $variables['link']['target'] = '_blank';
          }
        }
      }
    }
    catch (MissingDataException $e) {
      Drupal::logger('premium_theme')->error($e->getMessage());
    }
  }
}

/**
 * Change hero paragraph for layout builder, add media and page title.
 *
 * @param array $variables
 *   Theme variables.
 */
function premium_theme_preprocess_paragraph__inline_hero(array &$variables) {
  /** @var Paragraph $paragraph */
  $paragraph = $variables['elements']['#paragraph'];

  if ($paragraph->hasField('field_background') && !$paragraph->get('field_background')->isEmpty()) {
    $media = $paragraph->get('field_background')->entity;
    if (!is_null($media)) {
      $variables['media_image'] = \Drupal::entityTypeManager()->getViewBuilder('media')->view($media, 'hero');
      if (in_array($media->bundle(), ['remote_video', 'cdn_video'])) {
        $variables['media'] = \Drupal::entityTypeManager()->getViewBuilder('media')->view($media);
      }
    }
  }

  if ($paragraph->hasField('field_cta') && !$paragraph->get('field_cta')->isEmpty()) {
    try {
      $button = $paragraph->get('field_cta')->first();

      if (!is_null($button)) {
        $button = $button->getValue();

        if (isset($button['uri'], $button['title'])) {
          $variables['link']['title'] = $button['title'];
          $variables['link']['url'] = Url::fromUri($button['uri'])->toString();

          if (isset($button['target_blank']) && (int) $button['target_blank'] === 1) {
            $variables['link']['target'] = '_blank';
          }
        }
      }
    }
    catch (MissingDataException $e) {
      Drupal::logger('premium_theme')->error($e->getMessage());
    }
  }
}

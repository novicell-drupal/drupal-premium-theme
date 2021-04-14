<?php

use Drupal\Core\TypedData\Exception\MissingDataException;
use Drupal\Core\Url;

/**
 * Change hero paragraph, add media and page title.
 *
 * @param array $variables
 *   Theme variables.
 */
function premium_theme_preprocess_paragraph__basic_hero__full(array &$variables) {
  /** @var \Drupal\paragraphs\Entity\Paragraph $paragraphs */
  $paragraph = $variables['elements']['#paragraph'];
  $variables['title'] = $variables['elements']['#title'];

  if ($paragraph->hasField('field_background') && !$paragraph->get('field_background')->isEmpty()) {
    $variables['media'] = \Drupal::entityTypeManager()->getViewBuilder('media')->view($paragraph->get('field_background')->entity, 'hero');
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

<?php

use Drupal\Core\Url;
use Drupal\Core\TypedData\Exception\MissingDataException;

/**
 * Button layout block preprocess.
 *
 * @param array $variables
 *   Template variables.
 */
function premium_theme_preprocess_block__inline_block__button(array &$variables) {
  /** @var \Drupal\block_content\Entity\BlockContent $block */
  $block = $variables['elements']['content']['#block_content'];

  if ($block->hasField('field_button') && !$block->get('field_button')->isEmpty()) {
    try {
      $button = $block->get('field_button')->first();

      if (!is_null($button)) {
        $button = $button->getValue();

        if (isset($button['uri'], $button['title'])) {
          $variables['title'] = $button['title'];
          $variables['url'] = Url::fromUri($button['uri'])->toString();

          if ($block->hasField('field_button_alignment') && !$block->get('field_button_alignment')->isEmpty()) {
            $variables['alignment'] = $block->get('field_button_alignment')->value;
          }

          if (isset($button['target_blank']) && (int) $button['target_blank'] === 1) {
            $variables['target'] = '_blank';
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
 * Image layout block preprocess.
 *
 * @param array $variables
 *   Template variables.
 */
function premium_theme_preprocess_block__inline_block__image(array &$variables) {
  /** @var \Drupal\block_content\Entity\BlockContent $block */
  $block = $variables['elements']['content']['#block_content'];

  if ($block->hasField('field_image_media') && !$block->get('field_image_media')->isEmpty()) {
    $media = $block->get('field_image_media')->entity;
    $layout = $variables['elements']['content']['#layout'] ?? [];
    $image_style = '1_column';
    $layout_id = $layout['layout_id'] ?? '';

    if ($layout_id === 'layout_onecol' && isset($layout['settings']['column_width']) && $layout['settings']['column_width'] === 'fullwidth') {
      $image_style = 'full_width_16_9';
    }
    elseif ($layout_id === 'layout_twocol_section') {
      $image_style = '2_column';
    }
    elseif ($layout_id === 'layout_threecol_section') {
      $image_style = '3_column';
    }

    $variables['media'] = \Drupal::entityTypeManager()->getViewBuilder('media')->view($media, $image_style);
  }

  if ($block->hasField('field_image_link') && !$block->get('field_image_link')->isEmpty()) {
    try {
      $link = $block->get('field_image_link')->first();

      if (!is_null($link)) {
        $link = $link->getValue();

        if (isset($link['uri'])) {
          $variables['link']['url'] = Url::fromUri($link['uri'])->toString();

          if (isset($link['target_blank']) && (int) $link['target_blank'] === 1) {
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
 * Slideshow layout block preprocess.
 *
 * @param array $variables
 *   Template variables.
 */
function premium_theme_preprocess_layout_block__slideshow(array &$variables) {
  $items = $variables['fields']['field_slideshow_images'];

  if (!empty($items)) {
    foreach ($items as $item) {
      $variables['items'][] = ['image' => $item['field_media_image']];
    }
  }
}

/**
 * Hero layout block preprocess.
 *
 * @param array $variables
 *   Template variables.
 */
function premium_theme_preprocess_layout_block__hero(array &$variables) {
  $entity = $variables['entity'];
  $variables['hero'] = _create_hero($entity, [
    'title' => 'field_hero_headline',
    'media' => 'field_hero_background',
    'text_color' => 'field_hero_color',
    'orientation' => 'field_hero_orientation',
    'teaser' => 'field_hero_subline',
    'cta' => 'field_hero_button',
  ]);
}

/**
 * Video layout block preprocess.
 *
 * @param array $variables
 *   Template variables.
 */
function premium_theme_preprocess_block__inline_block__video(array &$variables) {
  /** @var \Drupal\block_content\Entity\BlockContent $block */
  $block = $variables['elements']['content']['#block_content'];

  if ($block->hasField('field_video_media') && !$block->get('field_video_media')->isEmpty()) {
    $media = $block->get('field_video_media')->entity;
    $layout = $variables['elements']['content']['#layout'] ?? [];
    $image_style = '1_column';
    $layout_id = $layout['layout_id'] ?? '';

    if ($layout_id === 'layout_onecol' && isset($layout['settings']['column_width']) && $layout['settings']['column_width'] === 'fullwidth') {
      $image_style = 'full_width_16_9';
    }
    elseif ($layout_id === 'layout_twocol_section') {
      $image_style = '2_column';
    }
    elseif ($layout_id === 'layout_threecol_section') {
      $image_style = '3_column';
    }

    $variables['media_image'] = \Drupal::entityTypeManager()->getViewBuilder('media')->view($media, $image_style);
  }
}


/**
 * Appetizer layout block preprocess.
 *
 * @param array $variables
 *   Template variables.
 */
function premium_theme_preprocess_layout_block__appetizer(array &$variables) {
  $entity = $variables['entity'];

  if ($entity->hasField('field_appetizer_cta') && !$entity->get('field_appetizer_cta')->isEmpty()) {
    $cta = $entity->get('field_appetizer_cta')->getValue();

    if (isset($cta['uri'], $cta['title'])) {
      $variables['cta']['title'] = $cta['title'];
      $variables['cta']['url'] = Url::fromUri($cta['uri'])->toString();

      if (isset($cta['target_blank']) && (int) $cta['target_blank'] === 1) {
        /** @var \Drupal\Core\Template\Attribute $cta_attributes */
        $cta_attributes = new Attribute();
        $cta_attributes->setAttribute('target', 'blank');
        $variables['cta']['attributes'] = $cta_attributes;
      }
    }
  }

  $layout_id = $variables['layout']['layout_id'];
  $layout_settings = $variables['layout']['settings'];

  if ($layout_id === 'layout_twocol_section' || $layout_id === 'layout_threecol_section') {
    $variables['is_small_version'] = TRUE;
  }

  if ($layout_id === 'layout_onecol' && isset($layout_settings['column_width']) && $layout_settings['column_width'] === 'fullwidth') {
    $variables['image_style'] = 'appetiser_wide';
    $variables['display_as_fullwidth'] = TRUE;
  }
  elseif ($layout_id === 'layout_onecol') {
    $variables['image_style'] = 'appetiser';
  }
  elseif ($layout_id === 'layout_twocol_section') {
    $variables['image_style'] = '2_column';
  }
  elseif ($layout_id === 'layout_threecol_section') {
    $variables['image_style'] = '3_column';
  }
  else {
    $variables['image_style'] = 'appetiser';
  }
}

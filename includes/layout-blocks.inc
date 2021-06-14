<?php

use Drupal\Core\Cache\Cache;
use Drupal\Core\Url;
use Drupal\Core\TypedData\Exception\MissingDataException;
use Drupal\node\Entity\Node;

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

    if (!is_null($media)) {
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

      if ($block->hasField('field_dynamic_height') && (int) $block->get('field_dynamic_height')->value === 1) {
        if ($image_style === 'full_width_16_9') {
          $image_style = 'full_width_flex_height';
        }
        else {
          $image_style .= '_flex_height';
        }
      }

      $variables['media'] = \Drupal::entityTypeManager()->getViewBuilder('media')->view($media, $image_style);
    }
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
function premium_theme_preprocess_block__inline_block__slideshow(array &$variables) {
  /** @var \Drupal\block_content\Entity\BlockContent $block */
  $block = $variables['elements']['content']['#block_content'];

  if ($block->hasField('field_slideshow_images') && !$block->get('field_slideshow_images')->isEmpty()) {
    $medias = $block->get('field_slideshow_images')->referencedEntities();
    $media_view_builder = \Drupal::entityTypeManager()->getViewBuilder('media');

    if (!empty($medias)) {
      foreach ($medias as $media) {
        $variables['items'][] = ['image' => $media_view_builder->view($media, 'hero')];
      }
    }
  }
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

    if (!is_null($media)) {
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
}

/**
 * Appetizer layout block preprocess.
 *
 * @param array $variables
 *   Template variables.
 */
function premium_theme_preprocess_block__inline_block__appetizer(array &$variables) {
  /** @var \Drupal\block_content\Entity\BlockContent $block */
  $block = $variables['elements']['content']['#block_content'];
  $layout = $variables['elements']['content']['#layout'] ?? [];
  $layout_id = $layout['layout_id'] ?? '';
  $image_style = 'appetiser';

  if ($block->hasField('field_appetizer_cta') && !$block->get('field_appetizer_cta')->isEmpty()) {
    try {
      $button = $block->get('field_appetizer_cta')->first();

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

  if ($layout_id === 'layout_twocol_section' || $layout_id === 'layout_threecol_section') {
    $variables['is_small_version'] = TRUE;
  }

  if ($layout_id === 'layout_onecol' && isset($layout['settings']['column_width']) && $layout['settings']['column_width'] === 'fullwidth') {
    $image_style = 'appetiser_wide';
    $variables['display_as_fullwidth'] = TRUE;
  }
  elseif ($layout_id === 'layout_onecol') {
    $image_style = 'appetiser';
  }
  elseif ($layout_id === 'layout_twocol_section') {
    $image_style = '2_column';
  }
  elseif ($layout_id === 'layout_threecol_section') {
    $image_style = '3_column';
  }

  if ($block->hasField('field_appetizer_image') && !$block->get('field_appetizer_image')->isEmpty() && !is_null($block->get('field_appetizer_image')->entity)) {
    $variables['media'] = \Drupal::entityTypeManager()->getViewBuilder('media')->view($block->get('field_appetizer_image')->entity, $image_style);
  }
}

/**
 * Code preprocess.
 *
 * @param array $variables
 *   Template variables.
 */
function premium_theme_preprocess_block__inline_block__code(array &$variables) {
  $variables['layout_builder_mode'] = strpos(Drupal::routeMatch()->getRouteName(), 'layout_builder') === 0;
}

/**
 * Inline navigation preprocess.
 *
 * @param array $variables
 *   Template variables.
 */
function premium_theme_preprocess_block__inline_block__inline_navigation(array &$variables) {
  /** @var \Drupal\block_content\Entity\BlockContent $block */
  $block = $variables['elements']['content']['#block_content'];

  $nodes = [];
  if (Drupal::moduleHandler()->moduleExists('content_hierarchy') && $block->hasField('field_inline_navigation_mode') && $block->get('field_inline_navigation_mode')->value === 'parent' && !empty($block->get('field_inline_navigation_parent')->target_id)) {
    $parent_id = $block->get('field_inline_navigation_parent')->target_id;
    $node = Node::load($parent_id);
    if (!empty($node)) {
      /** @var \Drupal\content_hierarchy\ContentHierarchyStorage $contentHierarchyStorage */
      $contentHierarchyStorage = \Drupal::service('content_hierarchy.storage');
      $content = $contentHierarchyStorage->loadFromEntity($node);
      if (!is_null($content)) {
        $children = $content->getChildren() ?? [];
        foreach ($children as $child) {
          $nodes[] = $child->getEntity();
        }

        // Set a cache tags, so it is possible to invalidate it,
        // if a new node sets the selected parent as it´s parent.
        if (!empty($variables['#cache']['tags'])) {
          $variables['#cache']['tags'] = Cache::mergeTags($variables['#cache']['tags'], $contentHierarchyStorage->getContentCacheTags([$content]));
        }
        else {
          $variables['#cache']['tags'] = $contentHierarchyStorage->getContentCacheTags([$content]);
        }
      }
    }
  }
  else {
    $node_ids = [];
    if ($block->hasField('field_inline_navigation_pages')) {
      foreach ($block->get('field_inline_navigation_pages') as $value) {
        $node_ids[] = $value->target_id;
      }
      $nodes = Node::loadMultiple($node_ids);
    }
  }

  if (!empty($nodes)) {
    $view_builder = \Drupal::entityTypeManager()->getViewBuilder('node');
    $rendered_nodes = [];
    /** @var \Drupal\node\Entity\Node $node */
    foreach ($nodes as $node) {
      if ($node->access()) {
        $rendered_nodes[] = $view_builder->view($node, 'inline_navigation');
      }
    }
    $variables['content'] = $rendered_nodes;
  }
  else {
    $variables['content'] = [];
  }

}

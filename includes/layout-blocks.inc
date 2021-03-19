<?php

/**
 * Button layout block preprocess.
 *
 * @param array $variables
 *   Template variables.
 */
function premium_theme_preprocess_layout_block__button(array &$variables) {
  $entity = $variables['entity'];
  if ($entity->hasField('field_button') && !$entity->get('field_button')->isEmpty()) {
    $cta = $entity->get('field_button')->getValue();
    /** @var \Drupal\Core\Template\Attribute $attribute */
    $attribute = new Attribute();
    if ($entity->hasField('field_button_alignment') && !$entity->get('field_button_alignment')->isEmpty()) {
      $attribute->addClass('button--' . $entity->get('field_button_alignment')->value);
    }
    $variables['cta'] = _modify_cta(reset($cta), $attribute);
  }
}

/**
 * Image layout block preprocess.
 *
 * @param array $variables
 *   Template variables.
 */
function premium_theme_preprocess_layout_block__image(array &$variables) {
  $layout_id = $variables['layout']['layout_id'];
  $layout_settings = $variables['layout']['settings'];

  if ($layout_id === 'layout_onecol' && $layout_settings['column_width'] === 'fullwidth') {
    $variables['image_style'] = 'full_width_16_9';
  }
  elseif ($layout_id === 'layout_onecol') {
    $variables['image_style'] = '1_column';
  }
  elseif ($layout_id === 'layout_twocol_section') {
    $variables['image_style'] = '2_column';
  }
  elseif ($layout_id === 'layout_threecol_section') {
    $variables['image_style'] = '3_column';
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
 * Accordion layout block preprocess.
 *
 * @param array $variables
 *   Template variables.
 */
function premium_theme_preprocess_layout_block__accordion(array &$variables) {
  $entity = $variables['entity'];
  $items = $entity->get('field_accordion_items');
  if (!empty($items)) {
    foreach ($items as $item) {
      $item = $item->entity;
      $variables['items'][] = [
        'id' => $item->id(),
        'headline' => $item->get('field_accordion_item_headline')->value,
        'text' => ['#markup' => $item->get('field_accordion_item_text')->value],
      ];
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
    'background_image' => 'field_hero_background',
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
function premium_theme_preprocess_layout_block__video(array &$variables) {
  $video_oembed = ['url' => 'null', 'oEmbed' => 'null'];
  $video_media = $variables['fields']['field_video_media'];
  if (!empty($video_media) && Drupal::hasService('dds_media.media_helper')) {
    /** @var Drupal\dds_media\MediaHelper $service */
    $service = Drupal::service('dds_media.media_helper');
    $video_data = $service->get_video_data_from_media($video_media->getEntity());
    $video_oembed['url'] = $video_data['url'];
    $video_oembed['oEmbed'] = ['html' => $video_data['player']];
  }
  $variables['video_oembed'] = json_encode($video_oembed);

  $layout_id = $variables['layout']['layout_id'];
  $layout_settings = $variables['layout']['settings'];

  if ($layout_id === 'layout_onecol' && isset($layout_settings['column_width']) && $layout_settings['column_width'] === 'fullwidth') {
    $variables['image_style'] = 'full_width_16_9';
  }
  elseif ($layout_id === 'layout_onecol') {
    $variables['image_style'] = '1_column';
  }
  elseif ($layout_id === 'layout_twocol_section') {
    $variables['image_style'] = '2_column';
  }
  elseif ($layout_id === 'layout_threecol_section') {
    $variables['image_style'] = '3_column';
  }
}


/**
 * Appetizer layout block preprocess.
 *
 * @param array $variables
 *   Template variables.
 */
function premium_theme_preprocess_layout_block__appetizer(array &$variables) {
  $variables['image_figure_attributes'] = new Attribute();

  $entity = $variables['entity'];
  if ($entity->hasField('field_appetizer_cta') && !$entity->get('field_appetizer_cta')->isEmpty()) {
    $cta = $entity->get('field_appetizer_cta')->getValue();
    /** @var \Drupal\Core\Template\Attribute $attribute */
    $attribute = new Attribute();
    $attribute->addClass('button--ghost');
    $variables['cta'] = _modify_cta(reset($cta), $attribute);
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

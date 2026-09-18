<?php

/**
 * Standalone regression checks: php tests/gin-lb-compatibility.php.
 */
class Drupal {
  public static bool $ginLb = FALSE;
  public static function moduleHandler(): object {
    return new class {
      public function moduleExists(string $name): bool {
        return $name === 'gin_lb' && Drupal::$ginLb;
      }
    };
  }
}
require dirname(__DIR__) . '/premium_theme.theme';
$check = static function (bool $condition, string $message): void {
  if (!$condition) {
    throw new RuntimeException($message);
  }
};
$suggestions = ['form__layout_builder_form__gin_lb'];
$variables = ['element' => ['#gin_lb_form' => TRUE, '#form_id' => 'node_page_layout_builder_form'], 'theme_hook_original' => 'form'];
premium_theme_theme_suggestions_form_alter($suggestions, $variables);
$check($suggestions === ['form__layout_builder_form__gin_lb'], 'Gin LB form lost template priority.');
unset($variables['element']['#gin_lb_form']);
premium_theme_theme_suggestions_form_alter($suggestions, $variables);
$check(end($suggestions) === 'form__node_page_layout_builder_form', 'Frontend form suggestion was lost.');
foreach (['checkbox', 'radio', 'textfield'] as $type) {
  $variables = ['element' => ['#type' => $type, '#gin_lb_form' => TRUE], 'attributes' => ['class' => ['glb-input']]];
  premium_theme_preprocess_input($variables);
  $check($variables['attributes']['class'] === ['glb-input'], 'Frontend classes leaked into Gin LB input.');
  unset($variables['element']['#gin_lb_form']);
  premium_theme_preprocess_input($variables);
  $check(count($variables['attributes']['class']) === 2, 'Frontend input class was lost.');
}
$variables = ['element' => ['#gin_lb_form' => TRUE], 'attributes' => ['class' => []]];
premium_theme_preprocess_textarea($variables);
$check($variables['attributes']['class'] === [], 'Frontend textarea class leaked into Gin LB.');
$libraries = [];
foreach (['dialog', 'layout-builder', 'layout-builder-form', 'layout-builder-form-confirmation', 'status-messages', 'lazyload'] as $name) {
  $libraries[$name] = ['css' => ['theme' => [$name . '.css' => []]], 'js' => [$name . '.js' => []]];
}
$original = $libraries;
premium_theme_library_info_alter($libraries, 'premium_theme');
$check($libraries === $original, 'Legacy editor changed without Gin LB.');
Drupal::$ginLb = TRUE;
premium_theme_library_info_alter($libraries, 'another_theme');
$check($libraries === $original, 'Another extension was changed.');
premium_theme_library_info_alter($libraries, 'premium_theme');
foreach (['dialog', 'layout-builder', 'layout-builder-form', 'layout-builder-form-confirmation'] as $name) {
  $check($libraries[$name]['css'] === [], 'Conflicting editor CSS was retained.');
  $check($libraries[$name]['js'] === $original[$name]['js'], 'JavaScript was removed.');
}
foreach (['status-messages', 'lazyload'] as $name) {
  $check($libraries[$name] === $original[$name], 'Non-editor library was changed.');
}
print "PASS: Gin LB suggestions/input classes and targeted editor library compatibility.\n";

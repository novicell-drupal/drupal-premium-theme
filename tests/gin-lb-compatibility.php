<?php

/**
 * @file
 * Standalone regression checks; no Drupal bootstrap or installed-site writes.
 */

namespace Composer {
  // Deliberately do not load Composer/Drupal: each case uses isolated metadata.
  if (!in_array('--without-composer', $argv, TRUE)) {
    class InstalledVersions {
      public static array $versions = [];

      public static function isInstalled($package): bool {
        return array_key_exists($package, self::$versions);
      }

      public static function getVersion($package): ?string {
        return self::$versions[$package];
      }
    }
  }
}

namespace {
  class Drupal {
    public static array $modules = [];

    public static function moduleHandler(): object {
      return new class {
        public function moduleExists(string $name): bool {
          return in_array($name, Drupal::$modules, TRUE);
        }
      };
    }
  }

  require dirname(__DIR__) . '/premium_theme.theme';

  function check(bool $condition, string $message): void {
    if (!$condition) {
      throw new \RuntimeException($message);
    }
  }

  function checkBehavior(bool $supported): void {
    check(_premium_theme_has_supported_gin_lb() === $supported, 'Incorrect compatibility decision.');
    // An unsupported marked form must behave like the legacy frontend form.
    foreach ([FALSE, TRUE] as $marked) {
      $gin_form = $supported && $marked;
      $suggestions = ['form__layout_builder_form__gin_lb'];
      $variables = [
        'element' => ['#gin_lb_form' => $marked, '#form_id' => 'node_page_layout_builder_form'],
        'theme_hook_original' => 'form',
      ];
      premium_theme_theme_suggestions_form_alter($suggestions, $variables);
      check(end($suggestions) === ($gin_form ? 'form__layout_builder_form__gin_lb' : 'form__node_page_layout_builder_form'), 'Wrong form template priority.');
      foreach (['checkbox' => 'checkbox', 'radio' => 'radio', 'textfield' => 'input'] as $type => $class) {
        $variables = ['element' => ['#type' => $type, '#gin_lb_form' => $marked], 'attributes' => ['class' => ['existing']]];
        premium_theme_preprocess_input($variables);
        check($variables['attributes']['class'] === ($gin_form ? ['existing'] : ['existing', $class]), 'Incorrect input classes: ' . $type);
      }
      $variables = ['element' => ['#gin_lb_form' => $marked], 'attributes' => ['class' => ['existing']]];
      premium_theme_preprocess_textarea($variables);
      check($variables['attributes']['class'] === ($gin_form ? ['existing'] : ['existing', 'textarea']), 'Incorrect textarea classes.');
    }

    $editor = ['dialog', 'layout-builder', 'layout-builder-form', 'layout-builder-form-confirmation'];
    $libraries = [];
    foreach (array_merge($editor, ['status-messages', 'lazyload']) as $name) {
      $libraries[$name] = ['css' => ['theme' => [$name . '.css' => []]], 'js' => [$name . '.js' => []]];
    }
    $original = $libraries;
    premium_theme_library_info_alter($libraries, 'another_theme');
    check($libraries === $original, 'Unrelated extension changed.');
    premium_theme_library_info_alter($libraries, 'premium_theme');
    foreach ($libraries as $name => $library) {
      $expected = $original[$name];
      if ($supported && in_array($name, $editor, TRUE)) {
        $expected['css'] = [];
      }
      check($library === $expected, 'Incorrect library handling: ' . $name);
    }

    $claro = [
      'claro.drupal.dialog' => ['css' => ['theme' => ['css/components/dialog.css' => []]], 'dependencies' => ['claro/variables']],
      'unrelated' => ['js' => ['unrelated.js' => []]],
    ];
    $expected = $claro;
    if (!$supported) {
      $expected['claro.drupal.dialog']['override'] = FALSE;
    }
    premium_theme_library_info_alter($claro, 'claro');
    check($claro === $expected, 'Incorrect Claro dialog fallback.');

    $core = ['drupal.dialog.off_canvas' => [
      'css' => ['base' => ['misc/dialog/off-canvas/css/reset.css' => [], 'misc/dialog/off-canvas/css/base.css' => []]],
      'js' => ['off-canvas.js' => []],
      'dependencies' => ['core/drupal'],
    ]];
    $expected = $core;
    if (!$supported) {
      unset($expected['drupal.dialog.off_canvas']['css']['base']['misc/dialog/off-canvas/css/reset.css']);
    }
    premium_theme_library_info_alter($core, 'core');
    check($core === $expected, 'Incorrect off-canvas reset fallback.');
    foreach (['premium_theme', 'claro', 'core'] as $extension) {
      $empty = [];
      premium_theme_library_info_alter($empty, $extension);
      check($empty === [], 'Missing libraries were created.');
    }
  }

  Drupal::$modules = ['gin_lb', 'gin_toolbar'];
  if (in_array('--without-composer', $argv, TRUE)) {
    checkBehavior(FALSE);
    print "PASS: unavailable Composer API retains all legacy behavior.\n";
    exit;
  }

  $baseline = ['drupal/gin' => '5.0.15.0', 'drupal/gin_toolbar' => '3.0.3.0', 'drupal/gin_lb' => '3.0.0.0-beta1'];
  $cases = [
    ['tested combination', [], TRUE],
    ['lowest Gin/Toolbar versions', ['drupal/gin' => '5.0.0.0', 'drupal/gin_toolbar' => '3.0.0.0'], TRUE],
    ['Gin LB beta2', ['drupal/gin_lb' => '3.0.0.0-beta2'], TRUE],
    ['Gin LB release candidate', ['drupal/gin_lb' => '3.0.0.0-RC1'], TRUE],
    ['Gin LB stable', ['drupal/gin_lb' => '3.0.0.0'], TRUE],
    ['compatible minor updates', ['drupal/gin' => '5.1.0.0', 'drupal/gin_toolbar' => '3.1.0.0', 'drupal/gin_lb' => '3.1.0.0'], TRUE],
    ['Gin 3', ['drupal/gin' => '3.0.0.0'], FALSE],
    ['Gin 4', ['drupal/gin' => '4.0.0.0'], FALSE],
    ['Gin 5 prerelease', ['drupal/gin' => '5.0.0.0-RC1'], FALSE],
    ['Gin 6 prerelease', ['drupal/gin' => '6.0.0.0-alpha1'], FALSE],
    ['Gin 6', ['drupal/gin' => '6.0.0.0'], FALSE],
    ['Toolbar 2', ['drupal/gin_toolbar' => '2.0.0.0'], FALSE],
    ['Toolbar 3 prerelease', ['drupal/gin_toolbar' => '3.0.0.0-beta1'], FALSE],
    ['Toolbar 4 prerelease', ['drupal/gin_toolbar' => '4.0.0.0-RC1'], FALSE],
    ['Toolbar 4', ['drupal/gin_toolbar' => '4.0.0.0'], FALSE],
    ['Gin LB 2', ['drupal/gin_lb' => '2.1.0.0'], FALSE],
    ['Gin LB early alpha', ['drupal/gin_lb' => '3.0.0.0-alpha1'], FALSE],
    ['Gin LB beta0', ['drupal/gin_lb' => '3.0.0.0-beta0'], FALSE],
    ['Gin LB 4 prerelease', ['drupal/gin_lb' => '4.0.0.0-beta1'], FALSE],
    ['Gin LB 4', ['drupal/gin_lb' => '4.0.0.0'], FALSE],
    ['unknown version', ['drupal/gin' => 'unknown'], FALSE],
    ['provided package without a version', ['drupal/gin' => NULL], FALSE],
    ['unversioned checkout', ['drupal/gin_lb' => 'dev-main'], FALSE],
    ['normalized development branch', ['drupal/gin' => '5.9999999.9999999.9999999-dev'], FALSE],
  ];
  foreach ($cases as [$name, $versions, $supported]) {
    \Composer\InstalledVersions::$versions = array_replace($baseline, $versions);
    try {
      checkBehavior($supported);
    }
    catch (\RuntimeException $exception) {
      throw new \RuntimeException($name . ': ' . $exception->getMessage(), 0, $exception);
    }
  }
  foreach (array_keys($baseline) as $missing) {
    \Composer\InstalledVersions::$versions = $baseline;
    unset(\Composer\InstalledVersions::$versions[$missing]);
    checkBehavior(FALSE);
  }
  \Composer\InstalledVersions::$versions = $baseline;
  foreach ([[], ['gin_lb'], ['gin_toolbar']] as $enabled) {
    Drupal::$modules = $enabled;
    checkBehavior(FALSE);
  }
  print "PASS: 30 version/enablement cases, marked/unmarked forms, editor assets and legacy dialog fallbacks.\n";
}

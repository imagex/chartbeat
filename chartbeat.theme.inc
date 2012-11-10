<?php

function theme_chartbeat_node_admin_nodes($form) {
  // If there are rows in this form, then $form['title'] contains a list of
  // the title form elements.
  $form['concurrent'] = _chartbeat_concurrent_api_call();

  $has_posts = isset($form['title']) && is_array($form['title']);
  $select_header = $has_posts ? theme('table_select_header_cell') : '';
  $header = array($select_header, t('Title'), t('Type'), t('Author'), t('Status'));
  if (isset($form['language'])) {
    $header[] = t('Language');
  }
  $header[] = t('Operations');
  $output = '';

  $output .= drupal_render($form['options']);
  if ($has_posts) {
    foreach (element_children($form['title']) as $key) {
      $row = array();
      $row[] = drupal_render($form['nodes'][$key]);
      $row[] = drupal_render($form['title'][$key]);
      $row[] = drupal_render($form['name'][$key]);
      $row[] = drupal_render($form['username'][$key]);
      $row[] = drupal_render($form['status'][$key]);
      if (isset($form['language'])) {
        $row[] = drupal_render($form['language'][$key]);
      }
      $row[] = drupal_render($form['operations'][$key]);
      $rows[] = $row;
    }

  }
  else {
    $rows[] = array(array(
        'data' => t('No posts available.'),
        'colspan' => '6',
      ));
  }

  $output .= theme('table', $header, $rows);
  if ($form['pager']['#value']) {
    $output .= drupal_render($form['pager']);
  }

  $output .= drupal_render($form);

  return $output;
}

function _chartbeat_concurrent_api_call(&$nid_array) {
  $api = variable_get('chartbeat_api_key', FALSE);

  $ret_array = array();

  foreach($nid_array as &$node) {

    $href = $node['title'];

    if ($api) {
      $host = $_SERVER['SERVER_NAME'];
      $req_http = "http://api.chartbeat.com/live/summary/v3/?apikey=$api&host=$host&keys=read&path=/$href";
      $resp = drupal_http_request($req_http);

      if ($resp->code == 200) {
        $response = json_decode($resp->data, true);
        if (!empty($response)) {
          $num = (int)$response['read']['data']['sum'];
          $node['concurrent'] = $num;
        } else {
          $node['concurrent'] = 0;
        }
      } else {
        $node['concurrent'] = 'n/a';
      }
    }
    else {
      $node['concurrent'] = 'n/a';
    }
  }

  return $ret_array;
}
--TEST--
Check for event configuration features 
--FILE--
<?php 
$cfg = new EventConfig();

if ($cfg->requireFeatures(EventConfig::FEATURE_FDS)) {
	$base = new EventBase($cfg);

	if ($base->getFeatures() & EventConfig::FEATURE_FDS) {
		echo "FDS\n";
	}
}

if ($cfg->requireFeatures(EventConfig::FEATURE_ET)) {
	$base = new EventBase($cfg);

	if ($base->getFeatures() & EventConfig::FEATURE_ET) {
		echo "ET\n";
	}
}

if ($cfg->requireFeatures(EventConfig::FEATURE_O1)) {
	$base = new EventBase($cfg);

	if ($base->getFeatures() & EventConfig::FEATURE_O1) {
		echo "O1\n";
	}
}

?>
--EXPECT--
FDS
ET
O1

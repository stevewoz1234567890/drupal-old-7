(function($) {

	function numberWithCommas(x) {
		return x.toString().replace(/\B(?=(\d{3})+(?!\d))/g, ",");
	}

	var ctx = $("#myChart"),
		totals = heartbeat.totals;
	Chart.defaults.global.legend.display = false;
	Chart.defaults.global.tooltips.callbacks.title = function(tooltipItems, data) {
		if (tooltipItems[0].datasetIndex === 0) {
			var timestamp = Math.floor(tooltipItems[0].index * 15);
			if (typeof totals[timestamp] !== 'undefined') {
				return tooltipItems[0].yLabel + '% (' + numberWithCommas(totals[timestamp]) + '/' + numberWithCommas(totals[0]) + ')';
			}
		} else if (tooltipItems[0].datasetIndex === 1) {
			return tooltipItems[0].yLabel + '% (Average)';
		}
		return tooltipItems[0].yLabel + '%';
	};
	Chart.defaults.global.tooltips.callbacks.label = function(tooltipItems, data) {
		var timestamp = Math.floor(tooltipItems.index * 15 / 60);

		switch (tooltipItems.index * 15 / 60 % 1) {
			case 0.25:
				timestamp = timestamp + ':15';
				break;
			case 0.5:
				timestamp = timestamp + ':30';
				break;
			case 0.75:
				timestamp = timestamp + ':45';
				break;
			default:
				timestamp = timestamp + ':00';
				break;
		}
		return timestamp;
	};
	var options = {
		scales: {
			yAxes: [{
				position: 'left',
				gridLines: {
					drawTicks: false
				},
				ticks: {
					min: 0,
					max: 100,
					callback: function(value) {
						return value + '%';
					}
				}
          }],
			xAxes: [{
					position: 'bottom',
					gridLines: {
						drawTicks: false
					},
					ticks: {
						autoSkip: false,
						maxRotation: 0,
						beginAtZero: true,
						callback: function(value) {
							if (value % 300 === 0) {
								return (value / 60) + ':00';
							} else {
								return null;
							}
						}
					}
            },
				{
					position: "top",
					gridLines: {
						color: '#02135B',
						lineWidth: 2
					},
					ticks: {
						autoSkip: false,
						beginAtZero: true,
						//maxRotation: 0,
						callback: function(value) {
							var acts = heartbeat.acts;
							if (typeof acts[value] !== 'undefined') {
								return acts[value];
							} else {
								return null;
							}
						}
					}
        }
      ]
		}
	};
	var data = {
		labels: heartbeat.labels,
		datasets: [
			{
				fill: true,
				lineTension: 0.1,
				backgroundColor: "rgba(226,67,41,0.4)",
				borderColor: "rgba(226,67,41,1)",
				borderCapStyle: 'butt',
				borderDash: [],
				borderDashOffset: 0.0,
				borderJoinStyle: 'miter',
				pointBorderColor: "rgba(226,67,41,1)",
				pointBackgroundColor: "#fff",
				pointBorderWidth: 0,
				pointHoverRadius: 0,
				pointHoverBorderWidth: 0,
				pointRadius: 0,
				pointHitRadius: 10,
				data: heartbeat.percentages
        }
      ]
	};
	if (typeof heartbeat.averages !== 'undefined') {
		var averages = {
			fill: false,
			lineTension: 0.1,
			backgroundColor: "rgba(226,67,41,0.4)",
			borderColor: "rgba(0,0,0,0.25)",
			borderCapStyle: 'butt',
			borderDash: [],
			borderDashOffset: 0.0,
			borderJoinStyle: 'miter',
			pointBorderColor: "rgba(226,67,41,1)",
			pointBackgroundColor: "#fff",
			pointBorderWidth: 0,
			pointHoverRadius: 0,
			pointHoverBorderWidth: 0,
			pointRadius: 0,
			pointHitRadius: 10,
			data: heartbeat.averages
		};
		data.datasets.push(averages);
	}
	var myLineChart = new Chart(ctx, {
		type: 'line',
		data: data,
		options: options
	});

	/* This is the end */

})(jQuery);
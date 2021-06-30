import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';

import * as olsource from 'ol/source';
import Map from 'ol/Map';
import TileLayer from 'ol/layer/Tile';
import View from 'ol/View';
import Feature from 'ol/Feature';
import { Point } from 'ol/geom';
import { Icon, Style } from 'ol/style';
import { fromLonLat } from 'ol/proj';
import { Vector } from 'ol/layer';

@Component({
  selector: 'location',
  templateUrl: './location.component.html',
  styleUrls: ['./location.component.css']
})
export class LocationComponent implements OnInit {

  constructor(private route: ActivatedRoute) { }

  device_name: string = '';
  latitude: number = 40.421488;
  longitude: number = -86.916631;

  map!: Map;
  layer!: Vector;

  ngOnInit(): void {
    this.device_name = this.route.snapshot.params.device_name;
    this.map = new Map({
      target: 'device_map',
      layers: [
        new TileLayer({
          source: new olsource.OSM()
        })
      ],
      view: new View({
        center: fromLonLat([
          this.longitude, 
          this.latitude
        ]),
        zoom: 15
      })
    });

    this.layer = new Vector({
      source: new olsource.Vector({
        features: [
          new Feature({
            geometry: new Point(fromLonLat([
              this.longitude, 
              this.latitude
            ]))
          })
        ]
      })
    });
    this.map.addLayer(this.layer);
  }

  changeLocation() {
    console.log("Latitude = " + this.latitude);
    console.log("Longitude = " + this.longitude);
  }
}
